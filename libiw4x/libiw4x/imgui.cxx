#include <libiw4x/imgui.hxx>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include <dbghelp.h>

#include <mutex>
#include <stdexcept>

// Forward declare the ImGui Win32 message handler. See imgui_impl_win32.h for
// context.
//
extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler (HWND hWnd,
                                UINT msg,
                                WPARAM wParam,
                                LPARAM lParam);

using namespace std;

namespace iw4x
{
  // Static member initialization
  //
  // This static instance is required for Win32/D3D9 API callbacks. This is not
  // a design choice but a technical necessity imposed by the platform APIs
  // we're intercepting.
  //
  imgui *imgui::instance_ (nullptr);

  // Implementation details
  //
  namespace
  {
    // Thread-local barrier to prevent recursion in create_device.
    //
    thread_local bool create_device_barrier;

    // Synchronization for one-time initialization.
    //
    std::once_flag vtable_hook_flag;
    std::once_flag create_device_hook_flag;
  }

  imgui::imgui ()
    : initialized_ (false),
      device_objects_created_ (false),
      demo_window_visible_ (true),
      window_handle_ (nullptr),
      d3d9_device_ (nullptr),
      original_reset_ (nullptr),
      original_endscene_ (nullptr),
      original_create_device_ (nullptr),
      original_window_procedure_ (nullptr),
      hooked_window_ (nullptr)
  {
    instance_ = this;
    intercept_d3d9 ();
  }

  void
  imgui::initialize (HWND window, IDirect3DDevice9 *device)
  {
    if (initialized_)
      return;

    window_handle_ = window;
    d3d9_device_ = device;

    IMGUI_CHECKVERSION ();
    ImGui::CreateContext ();

    ImGuiIO &io (ImGui::GetIO ());
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.MouseDrawCursor = true;

    ImGui::StyleColorsDark ();

    if (!ImGui_ImplWin32_Init (window_handle_))
    {
      ImGui::DestroyContext ();
      return;
    }

    if (!ImGui_ImplDX9_Init (d3d9_device_))
    {
      ImGui_ImplWin32_Shutdown ();
      ImGui::DestroyContext ();
      return;
    }

    device_objects_created_ = true;
    initialized_ = true;

    intercept_window (window);
  }

  bool
  imgui::process_message (HWND window,
                          unsigned int message,
                          unsigned long long wparam,
                          long long lparam) const
  {
    if (!initialized_)
      return false;

    // clang-format off
    return ImGui_ImplWin32_WndProcHandler (window, message, wparam, lparam) != 0;
    // clang-format on
  }

  void
  imgui::render ()
  {
    if (!initialized_)
      return;

    ImGui_ImplDX9_NewFrame ();
    ImGui_ImplWin32_NewFrame ();
    ImGui::NewFrame ();

    ImGui::ShowDemoWindow (&demo_window_visible_);

    ImGui::EndFrame ();
    ImGui::Render ();
    ImGui_ImplDX9_RenderDrawData (ImGui::GetDrawData ());
  }

  void
  imgui::reset_device ()
  {
    if (initialized_ && device_objects_created_)
    {
      ImGui_ImplDX9_InvalidateDeviceObjects ();
      device_objects_created_ = false;
    }
  }

  void
  imgui::create_device_objects ()
  {
    if (initialized_ && !device_objects_created_)
    {
      ImGui_ImplDX9_CreateDeviceObjects ();
      device_objects_created_ = true;
    }
  }

  bool
  imgui::is_initialized () const
  {
    return initialized_;
  }

  bool
  imgui::has_device_objects () const
  {
    return device_objects_created_;
  }

  void
  imgui::intercept_d3d9 ()
  {
    void *direct3d_create9_slot (nullptr);

    // Passing nullptr to GetModuleHandle means "give me the handle for the
    // file that started this process", in practice, this is the image base
    // address in memory.
    //
    HMODULE handle (GetModuleHandle (nullptr));

    // ImageNtHeader() takes a pointer to the DOS header and finds
    // the NT header using the e_lfanew offset. From here on, we have
    // a full view of section headers and data directories.
    //
    PIMAGE_NT_HEADERS nt_headers (
      ImageNtHeader (reinterpret_cast<PIMAGE_DOS_HEADER> (handle)));

    // For a loaded PE image in memory, RVAs can be simply converted to
    // pointers by adding them to the module base address, since the loader
    // has already mapped the sections to their virtual addresses.
    //
    auto rva_to_pointer = [handle] (unsigned long rva) -> void *
    {
      return reinterpret_cast<char *> (handle) + rva;
    };

    // The OptionalHeader's DataDirectory table has an entry for the import
    // table (IMAGE_DIRECTORY_ENTRY_IMPORT). Its VirtualAddress is an RVA
    // pointing to the first IMAGE_IMPORT_DESCRIPTOR in the array. Each
    // descriptor represents one DLL that this module imports from.
    //
    PIMAGE_IMPORT_DESCRIPTOR import_descriptor (
      static_cast<PIMAGE_IMPORT_DESCRIPTOR> (rva_to_pointer (
        nt_headers->OptionalHeader.DataDirectory [IMAGE_DIRECTORY_ENTRY_IMPORT]
          .VirtualAddress)));

    // Walk each imported DLL until we find d3d9.dll.
    //
    for (; import_descriptor->Name != 0; ++import_descriptor)
    {
      char *dll_name (
        static_cast<char *> (rva_to_pointer (import_descriptor->Name)));

      // Case-insensitive match because the import name may differ in
      // case between the linker and the loader.
      //
      if (_stricmp (dll_name, "d3d9.dll") == 0)
      {
        // Once we have the right DLL, we need both the name table
        // (OriginalFirstThunk) and the address table (FirstThunk).
        //
        // OriginalFirstThunk is the lookup table the loader uses to find the
        // right address at load time, it points to IMAGE_IMPORT_BY_NAME
        // structures that store the function names.
        //
        // FirstThunk is the table that actually gets patched by the loader with
        // the resolved addresses from the target DLL, this is the IAT we're
        // going to patch.
        //
        IMAGE_THUNK_DATA *original_first_thunk (
          static_cast<IMAGE_THUNK_DATA *> (
            rva_to_pointer (import_descriptor->OriginalFirstThunk)));

        IMAGE_THUNK_DATA *first_thunk (static_cast<IMAGE_THUNK_DATA *> (
          rva_to_pointer (import_descriptor->FirstThunk)));

        // We stop when AddressOfData is zero (end of import list). For
        // ordinals (high bit set), there's no name to check, so we skip
        // those.
        //
        for (; original_first_thunk->u1.AddressOfData != 0;
             ++original_first_thunk, ++first_thunk)
        {
          if ((original_first_thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) == 0U)
          {
            // Translate AddressOfData to an IMAGE_IMPORT_BY_NAME.
            //
            IMAGE_IMPORT_BY_NAME *import_by_name (
              static_cast<IMAGE_IMPORT_BY_NAME *> (
                rva_to_pointer (original_first_thunk->u1.AddressOfData)));

            // Check for the symbol we want, this must match exactly,
            // because IAT slots are per-function.
            //
            if (strcmp (reinterpret_cast<const char *> (import_by_name->Name),
                        "Direct3DCreate9") == 0)
            {
              // Found it, store the address of the live function pointer
              // slot. This is where the loader wrote the real address,
              // and where we'll write ours.
              //
              direct3d_create9_slot = &first_thunk->u1.Function;
              break;
            }
          }
        }
        break;
      }
    }

    if (direct3d_create9_slot == nullptr)
      throw runtime_error ("unable to locate Direct3DCreate9 slot");

    intercept_vtable (direct3d_create9_slot,
                      reinterpret_cast<void *> (d3d9_create));
  }

  // Return a pointer to the first entry of the vtable for a given COM object.
  // The result is the "raw" table of virtual function pointers as laid out by
  // the Windows COM ABI. While this is not sanctioned by the C++ standard (and
  // is therefore, strictly speaking, undefined behaviour in pure ISO C++), the
  // COM ABI has been frozen for decades and every known Windows compiler (MSVC,
  // Clang/LLVM, GCC/MinGW) uses the same layout:
  //
  //   [object memory]
  //   +0x00  -> pointer to vtable (array of function pointers)
  //
  // In other words, any COM interface pointer is in fact a pointer to an object
  // whose first and only data member is the vtable pointer. Because COM
  // interfaces are pure-virtual and never multiply-inherited, this mapping is
  // stable across compiler versions and optimization levels.
  //
  // Note: This is not portable to non-Windows or non-COM objects, and it will
  // explode (figuratively) if passed a pointer that is not in fact a COM
  // interface instance.
  //
  void **
  imgui::get_vtable (void *object)
  {
    return *static_cast<void ***> (object);
  }

  void
  imgui::intercept_vtable (void *address, void *new_value, size_t size)
  {
    unsigned long o (0);

    VirtualProtect (address, size, PAGE_READWRITE, &o);
    *static_cast<void **> (address) = new_value;
    VirtualProtect (address, size, o, &o);
  }

  void
  imgui::intercept_window (HWND window)
  {
    if (hooked_window_ != window)
    {
      hooked_window_ = window;

      original_window_procedure_ = reinterpret_cast<WNDPROC> (
        SetWindowLongPtr (window,
                          GWLP_WNDPROC,
                          reinterpret_cast<LONG_PTR> (window_procedure)));
    }
  }

  IDirect3D9 *WINAPI
  imgui::d3d9_create (unsigned int sdk_version)
  {
    // The first operation retrieves the original Direct3DCreate9 export from
    // the system's d3d9.dll using GetProcAddress on an explicit GetModuleHandle
    // call. This is intentional: binding the symbol manually guarantee that we
    // always forward to the exact version in use by IW4 without inadvertently
    // patching all consumers through global IAT changes.
    //
    auto original_function (
      function_cast<IDirect3D9 *(WINAPI *) (unsigned int)> (
        GetProcAddress (GetModuleHandle ("d3d9.dll"), "Direct3DCreate9")));

    // Calling the resolved function produces the concrete IDirect3D9 COM
    // interface instance as constructed by the runtime. At this stage the
    // runtime has already allocated the object and populated its virtual
    // function table. The returned interface is functionally identical to what
    // IW4 would have received without us, and this moment represents our only
    // opportunity to observe and alter its vtable before any calls are made
    // through it.
    //
    IDirect3D9 *d3d9_interface (original_function (sdk_version));

    // From this interface we extract the vtable pointer, exploiting the fact
    // that COM guarantees the first word of any interface pointer is the
    // address of its vtable. There is no sanctioned mechanism for intercepting
    // COM method calls; altering the vtable in place is the only practical
    // method for inserting ourselves into the dispatch path. For context,
    // refer to get_vtable documentatinon.
    //
    void **vtable (instance_->get_vtable (d3d9_interface));

    // The original CreateDevice pointer is captured exactly once. Without this
    // preservation step, we would have no means of forwarding calls to the real
    // implementation and the graphics subsystem would fail to initialize.
    //
    std::call_once (create_device_hook_flag,
                    [vtable]
    {
      instance_->original_create_device_ =
        reinterpret_cast<create_device_function> (vtable [16]);
    });

    intercept_vtable (static_cast<void *> (&vtable [16]),
                      reinterpret_cast<void *> (d3d9_create_device));

    // The modified interface is returned to the caller. To IW4, this object is
    // indistinguishable from the original, but under the hood the CreateDevice
    // path is now routed through our code, granting us the ability to perform
    // further interception on the resulting device object.
    //
    return d3d9_interface;
  }

  HRESULT WINAPI
  imgui::d3d9_create_device (IDirect3D9 *d3d9,
                             unsigned int adapter,
                             D3DDEVTYPE device_type,
                             HWND focus_window,
                             unsigned long behavior_flags,
                             D3DPRESENT_PARAMETERS *presentation_parameters,
                             IDirect3DDevice9 **device)
  {
    // The first action is to guard against recursion. That is, as we replaces
    // the original CreateDevice in the IDirect3D9 vtable, any code path that
    // indirectly calls CreateDevice from here could re-enter our own
    // interceptor.
    //
    if (create_device_barrier)
    {
      if (instance_->original_create_device_ != nullptr)
      {
        return instance_->original_create_device_ (d3d9,
                                                   adapter,
                                                   device_type,
                                                   focus_window,
                                                   behavior_flags,
                                                   presentation_parameters,
                                                   device);
      }
      return E_FAIL;
    }

    create_device_barrier = true;
    auto barrier_guard ([&]
    {
      create_device_barrier = false;
    });

    // If for some reason the original CreateDevice pointer was never captured,
    // it's impossible to proceed. This is a hard failure condition, as there is
    // no valid fallback rendering path without a functional device.
    //
    if (instance_->original_create_device_ == nullptr)
      return E_FAIL;

    // The original CreateDevice is invoked here to construct the actual
    // IDirect3DDevice9 instance. At this point we still have no handle on the
    // device object; our interception capability is limited to the factory
    // method.
    //
    HRESULT result (instance_->original_create_device_ (d3d9,
                                                        adapter,
                                                        device_type,
                                                        focus_window,
                                                        behavior_flags,
                                                        presentation_parameters,
                                                        device));

    // If device creation failed or no device was returned, there is nothing
    // further to hook and we simply propagate the result code.
    //
    if (SUCCEEDED (result) && device != nullptr && *device != nullptr)
    {
      void **vtable (instance_->get_vtable (*device));

      // The original Reset and EndScene method pointers are preserved exactly
      // once, following the same reasoning applied in d3d9_create for
      // CreateDevice. These methods occupy vtable indices 16 and 42
      // respectively under the current ABI. As with CreateDevice, these
      // indices are implicit assumptions and any deviation in the COM layout
      // will invalidate our interception logic.
      //
      std::call_once (vtable_hook_flag,
                      [vtable]
      {
        instance_->original_reset_ =
          reinterpret_cast<reset_function> (vtable [16]);
        instance_->original_endscene_ =
          reinterpret_cast<endscene_function> (vtable [42]);
      });

      // Reset
      //
      intercept_vtable (static_cast<void *> (&vtable [16]),
                        reinterpret_cast<void *> (d3d9_reset));

      // EndScene
      //
      intercept_vtable (static_cast<void *> (&vtable [42]),
                        reinterpret_cast<void *> (d3d9_endscene));
    }

    // The HRESULT from the original CreateDevice is returned unchanged to
    // preserve the expected API contract.
    //
    return result;
  }

  HRESULT WINAPI
  imgui::d3d9_reset (IDirect3DDevice9 *device,
                     D3DPRESENT_PARAMETERS *presentation_parameters)
  {
    // The reset hook is our opportunity to gracefully survive a Direct3D device
    // reset. If we do not release ImGui's device-dependent state here, the
    // subsequent Direct3D reset would invalidate it, leaving us with dangling
    // GPU handles. Thus we deliberately tear down these resources now so that
    // they can be rebuilt later.
    //
    instance_->reset_device ();

    // We forward the reset request to the original device method, since it's
    // the implementation that must actually perform the state transition.
    //
    HRESULT result (
      instance_->original_reset_ (device, presentation_parameters));

    // Only if the underlying device reset succeeded do we rebuild our rendering
    // state. That is, we must avoids creating GPU resources against a device
    // that is in an undefined state, which would at best be wasted work and at
    // worst explode.
    //
    if (SUCCEEDED (result))
      instance_->create_device_objects ();

    return result;
  }

  HRESULT WINAPI
  imgui::d3d9_endscene (IDirect3DDevice9 *device)
  {
    // ImGui's D3D9 backend binds itself to a specific device instance. If the
    // device pointer we receive here differs from the one we have stored, it
    // means either the device was recreated or we are now being called from a
    // different rendering context. In both cases, we must reinitialize our
    // ImGui integration against the new device, as GPU resources and state
    // objects are tied to that exact instance.
    //
    if (instance_->d3d9_device_ != device)
    {
      instance_->d3d9_device_ = device;

      // To correctly initialize, we need the device's creation parameters,
      // specifically its focus window handle. This is the anchor to properly
      // integrate into IW4's message loop.
      //
      if (D3DDEVICE_CREATION_PARAMETERS creation_parameters {};
          SUCCEEDED (device->GetCreationParameters (&creation_parameters)))
        instance_->initialize (creation_parameters.hFocusWindow, device);
    }

    // Once initialized, we render ImGui's frame for the current scene. We
    // intentionally do this *before* delegating to the original EndScene to
    // guarantee our UI overlays are drawn atop whatever IW4 has rendered.
    // Placing this after EndScene would be too late, that is, the swap chain
    // would be already presented.
    //
    if (instance_->is_initialized ())
      instance_->render ();

    // The original EndScene is called to allow IW4 to complete its normal
    // rendering pipeline. Skipping this call would freeze or corrupt the
    // output, while calling it twice would double-present frames and break
    // timing.
    //
    return (instance_->original_endscene_ != nullptr)
      ? instance_->original_endscene_ (device)
      : D3D_OK;
  }

  long long CALLBACK
  imgui::window_procedure (HWND window,
                           unsigned int message,
                           unsigned long long wparam,
                           long long lparam)
  {
    // The custom window procedure is our point of interception for Win32
    // messages. We first offer ImGui the chance to consume the event outright,
    // since it implements its own internal event routing and may wish to
    // prevent IW4 from ever seeing certain messages (for example, character
    // input when a text field is active).
    //
    if (instance_->process_message (window, message, wparam, lparam))
      return 0;

    // If ImGui is active and has indicated interest in either mouse or keyboard
    // input, we short-circuit here to prevent IW4 from acting on the same
    // input. This guarantee, for instance, that moving the mouse over an ImGui
    // widget does not also move the in-game camera, or that pressing a key to
    // rename a profile does not simultaneously trigger an in-game action.
    //
    if (instance_->is_initialized ())
    {
      ImGuiIO &io (ImGui::GetIO ());

      if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) &&
          io.WantCaptureMouse)
        return 0;

      // FIXME: IW4 polls mouse position instead of handling WM_MOUSEMOVE.
      //
      if (message == WM_MOUSEMOVE && io.WantCaptureMouse)
        return 0;

      if ((message >= WM_KEYFIRST && message <= WM_KEYLAST) &&
          io.WantCaptureKeyboard)
        return 0;
    }

    // Messages that ImGui neither consumes nor captures are passed along to the
    // original window procedure to preserve IW4's native behavior. In the
    // absence of a valid original procedure, we defer to DefWindowProc, which
    // implements the default behavior for unhandled messages in the Win32 API.
    //
    if (instance_->original_window_procedure_ != nullptr)
      return CallWindowProc (instance_->original_window_procedure_,
                             window,
                             message,
                             wparam,
                             lparam);

    return DefWindowProc (window, message, wparam, lparam);
  }
}
