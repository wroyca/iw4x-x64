#include <libiw4x/imgui.hxx>

#include <d3d9.h>

#ifdef _MSC_VER
#  pragma comment(lib, "d3d9.lib")
#endif

#include <imgui.h>
#include <imgui/backends/imgui_impl_dx9.h>
#include <imgui/backends/imgui_impl_win32.h>

#include <libiw4x/utility-win32.hxx>

// Forward declare the ImGui Win32 message handler. See imgui_impl_win32.h for
// context.
//
extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler (HWND hWnd,
                                UINT msg,
                                WPARAM wParam,
                                LPARAM lParam);

namespace iw4x
{
  namespace
  {
    once_flag imgui_init;

    WNDPROC original_wnd_proc_ (nullptr);
  }

  imgui::imgui (renderer& r)
  {
    r.on_frame ().connect ([this] (IDirect3DDevice9* device)
    {
      // Initialize ImGui lazily on the first frame. We avoid frontloading this
      // work during renderer construction since we need a valid device anyway,
      // and there is no point in doing it twice if the first frame never
      // arrives.
      //
      call_once (imgui_init, [device] ()
      {
        HRESULT hr;

        // To integrate ImGui we need the window handle that owns the device.
        // Direct3D does not hand this out directly; the swap chain is the
        // sanctioned path. The detour feels roundabout but avoids maintaining
        // parallel knowledge of windowing state.
        //
        IDirect3DSwapChain9* swap_chain;
        hr = device->GetSwapChain (0, &swap_chain);
        if (FAILED (hr))
          throw runtime_error ("unable to retrieve DirectX swap chain");

        D3DPRESENT_PARAMETERS pp;
        hr = swap_chain->GetPresentParameters (&pp);
        swap_chain->Release ();
        if (FAILED (hr))
          throw runtime_error ("unable to retrieve presentation parameters");

        HWND hwnd (pp.hDeviceWindow);
        if (hwnd == nullptr)
        {
          // In some cases the device omits its owning window. Falling back to
          // the foreground window is not strictly correct, but it lets us keep
          // running without introducing a second initialization path. If this
          // proves insufficient in practice, the renderer will need to grow a
          // proper mechanism for supplying the window explicitly.
          //
          cerr << "warning: device window could not be determined; "
               << "defaulting to current foreground window" << endl;

          hwnd = GetForegroundWindow ();
        }

        // With the handle resolved we can commit to an ImGui context. Input and
        // docking are enabled up front to avoid layering configuration later.
        // Note that we rely on ImGui's defaults for most behavior.
        //
        IMGUI_CHECKVERSION ();
        ImGui::CreateContext ();
        ImGuiIO& io (ImGui::GetIO ());
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.MouseDrawCursor = true;

        ImGui::StyleColorsDark ();

        // The platform and renderer backends form the actual bridge. Without
        // both in place the context is inert. We fail fast on errors rather
        // than attempting recovery, since partial initialization would only
        // complicate the renderer state machine.
        //
        if (!ImGui_ImplWin32_Init (hwnd))
          throw runtime_error ("unable to initialize ImGui Win32 backend");

        if (!ImGui_ImplDX9_Init (device))
          throw runtime_error ("unable to initialize DirectX9 backend");

        // Hook window procedure to forward messages to ImGui.
        //
        original_wnd_proc_ = reinterpret_cast<WNDPROC> (
          SetWindowLongPtrW (hwnd,
                             GWLP_WNDPROC,
                             reinterpret_cast<LONG_PTR> (wnd_proc)));

        if (original_wnd_proc_ == nullptr)
        {
          ostringstream s;
          unsigned long e (GetLastError ());

          s << "unable to hook window procedure: "
            << format_message (e) << endl;

          throw runtime_error (s.str ());
        }
      });

      render_frame (device);
    });
  }

  void
  imgui::render_frame (IDirect3DDevice9* device)
  {
    // Start the ImGui frame
    //
    ImGui_ImplDX9_NewFrame ();
    ImGui_ImplWin32_NewFrame ();
    ImGui::NewFrame ();

    {
      static bool show_demo_window (true);
      ImGui::ShowDemoWindow (&show_demo_window);
    }

    // Rendering closes the loop by handing ImGui's draw data back to DirectX.
    // At this point the integration is complete for the frame: ImGui advances
    // in lockstep with the renderer without maintaining separate lifecycles.
    //
    ImGui::Render ();
    ImGui_ImplDX9_RenderDrawData (ImGui::GetDrawData ());
  }

  LRESULT CALLBACK
  imgui::wnd_proc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
  {
    // Forward the message to ImGui first. If ImGui handles it, we're done.
    // Otherwise, we pass it through to the original window procedure.
    //
    LRESULT imgui_wnd_proc (ImGui_ImplWin32_WndProcHandler (hwnd,
                                                            msg,
                                                            wparam,
                                                            lparam));
    if (imgui_wnd_proc != 0)
      return imgui_wnd_proc;

    // Fallback to iw4mp window procedure if ImGui doesn't handle the
    // message.
    //
    if (original_wnd_proc_ != nullptr)
      return CallWindowProcW (original_wnd_proc_, hwnd, msg, wparam, lparam);

    // If no original procedure was stored, fall back to Windows default
    // handler.
    //
    return DefWindowProcW (hwnd, msg, wparam, lparam);
  }
}
