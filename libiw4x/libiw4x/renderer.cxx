#include <libiw4x/renderer.hxx>

#include <cassert>
#include <iostream>
#include <ostream>
#include <sstream>

#include <d3d9.h>
#ifdef _MSC_VER
#  pragma comment(lib, "d3d9.lib")
#endif

#include <libiw4x/utility/utility-win32.hxx>

using namespace std;

namespace iw4x
{
  namespace
  {
    // Global state for the renderer instance and original function pointers.
    //
    renderer* inst (nullptr);

    using endscene_fn      = HRESULT (APIENTRY*) (IDirect3DDevice9*);
    using create_device_fn = HRESULT (APIENTRY*) (IDirect3D9*,
                                                  UINT,
                                                  D3DDEVTYPE,
                                                  HWND,
                                                  DWORD,
                                                  D3DPRESENT_PARAMETERS*,
                                                  IDirect3DDevice9**);

    endscene_fn      orig_endscene      (nullptr);
    create_device_fn orig_create_device (nullptr);

    // Change page protection and perform operation.
    //
    template <typename F>
    bool
    with_protection (void* addr, size_t sz, DWORD prot, F f)
    {
      DWORD op (0);
      if (VirtualProtect (addr, sz, prot, &op) == 0)
      {
        DWORD e (GetLastError ());
        ostringstream s;

        s << "unable to change page protection: "
          << format_message (e);

        throw runtime_error (s.str ());
      }

      // Perform the operation.
      //
      f ();

      // Restore protection. Don't fail on this; just warn.
      //
      if (VirtualProtect (addr, sz, op, &op) == 0)
      {
        DWORD e (GetLastError ());

        cerr << "warning: failed to restore page protection: "
             << format_message (e);
      }

      return true;
    }
  }

  renderer::renderer ()
  {
    // Obtain the d3d9 module handle. Since the process has already loaded
    // d3d9.dll (either directly or transitively), GetModuleHandle should
    // succeed. If it doesn't, we have an undocumented edge case.
    //
    HMODULE dm (GetModuleHandle ("d3d9"));
    if (dm == nullptr)
    {
      DWORD e (GetLastError ());
      ostringstream s;

      s << "unable to get d3d9 module handle: "
        << format_message (e);

      throw runtime_error (s.str ());
    }

    FARPROC dc (GetProcAddress (dm, "Direct3DCreate9"));
    if (dc == nullptr)
    {
      DWORD e (GetLastError ());
      ostringstream s;

      s << "unable to get Direct3DCreate9 address: "
        << format_message (e);

      throw runtime_error (s.str ());
    }

    // Create a temporary Direct3D interface to extract its vtable. Note that we
    // must release it afterwards to avoid leaking references.
    //
    using create_fn = IDirect3D9*(WINAPI*) (UINT);
    IDirect3D9* di (reinterpret_cast<create_fn> (dc) (D3D_SDK_VERSION));

    if (di == nullptr)
      throw runtime_error ("unable to create IDirect3D9 interface");

    // Extract the vtable pointer. In the COM ABI, the first member of any
    // interface object is the vtable pointer (an array of function pointers).
    //
    // Layout:
    //   [object] +0x00 -> vtable pointer
    //   [vtable] +0x00 -> QueryInterface
    //            +0x04 -> AddRef
    //            +0x08 -> Release
    //            ...
    //            +0x40 -> CreateDevice (vtable[16])
    //
    void** vt (*reinterpret_cast<void***> (di));

    // Hook CreateDevice (vtable entry 16).
    //
    // Note: In scenarios involving delay-loaded DLLs, restoring protection
    // may cause access violations if the loader writes to the same memory
    // after we've restored it. This is rare and we adopt a best-effort
    // approach. If issues arise, coordination with the loader is required.
    //
    bool create_device (with_protection (&vt [16],
                                         sizeof (void*),
                                         PAGE_EXECUTE_READWRITE,
                                         [&vt] ()
    {
      // Save the original CreateDevice pointer.
      //
      orig_create_device = reinterpret_cast<create_device_fn> (vt [16]);

      // Install our hook.
      //
      vt [16] = reinterpret_cast<void*> (+[] (IDirect3D9* d3d,
                                              UINT a,
                                              D3DDEVTYPE dt,
                                              HWND hw,
                                              DWORD bf,
                                              D3DPRESENT_PARAMETERS* pp,
                                              IDirect3DDevice9** dv) -> HRESULT
      {
        // Call the original CreateDevice first.
        //
        HRESULT hr (orig_create_device (d3d, a, dt, hw, bf, pp, dv));

        if (SUCCEEDED (hr))
        {
          // Extract the device vtable and hook EndScene (entry 42).
          //
          void** dvt (*reinterpret_cast<void***> (*dv));

          with_protection (&dvt [42],
                           sizeof (void*),
                           PAGE_EXECUTE_READWRITE,
                           [&dvt] ()
          {
            orig_endscene = reinterpret_cast<endscene_fn> (dvt [42]);

            dvt [42] =
              reinterpret_cast<void*> (+[] (IDirect3DDevice9* d) -> HRESULT
            {
              // Dispatch frame notification before calling original.
              //
              dispatch_frame (d);

              return orig_endscene (d);
            });
          });
        }

        return hr;
      });
    }));

    // Release the temporary interface.
    //
    di->Release ();

    if (!create_device)
      throw runtime_error ("unable to hook CreateDevice");

    // Register this instance as the active renderer.
    //
    inst = this;
  }

  renderer::signal&
  renderer::on_frame () noexcept
  {
    return frame_signal_;
  }

  void
  renderer::notify (IDirect3DDevice9* d)
  {
    assert (d != nullptr);

    // Fire the signal to notify all connected slots. Note that if any slot
    // throws, we terminate to avoid undefined behavior.
    //
    try
    {
      frame_signal_ (d);
    }
    catch (const exception& e)
    {
      ostringstream s;

      s << "exception in frame callback: "
        << e.what ();

      throw runtime_error (s.str ());
    }
    catch (...)
    {
      throw runtime_error ("unknown exception in frame callback");
    }
  }

  void
  dispatch_frame (IDirect3DDevice9* d)
  {
    if (inst != nullptr)
      inst->notify (d);
  }
}
