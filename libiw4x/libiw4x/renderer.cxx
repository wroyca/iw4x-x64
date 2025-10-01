#include <libiw4x/renderer.hxx>

#include <d3d9.h>

#ifdef _MSC_VER
#  pragma comment(lib, "d3d9.lib")
#endif

#include <libiw4x/utility-win32.hxx>

namespace iw4x
{
  namespace
  {
    renderer* instance (nullptr);

    using endscene_t      = HRESULT (APIENTRY*) (IDirect3DDevice9*);
    using create_device_t = HRESULT (APIENTRY*) (IDirect3D9*,
                                                 UINT,
                                                 D3DDEVTYPE,
                                                 HWND,
                                                 DWORD,
                                                 D3DPRESENT_PARAMETERS*,
                                                 IDirect3DDevice9**);

    endscene_t endscene (nullptr);
    create_device_t create_device (nullptr);
  }

  renderer::renderer ()
  {
    // IW4x is brought in implicitly by the process, which in turn ensures that
    // d3d9 is already resident. If it weren't, import resolution would have
    // failed long before execution reached this point.
    //
    // Given this chain of guarantees, it's debatable whether GetModuleHandle
    // can legitimately fail here at all. Should it return null, we are left
    // with an undocumented edge case, the API offers no contract to explain
    // what (if anything) went wrong.
    //
    HMODULE d3d9_module (GetModuleHandle ("d3d9"));
    FARPROC d3d9_create (GetProcAddress (d3d9_module, "Direct3DCreate9"));

    // A temporary Direct3D interface is created here solely to obtain its
    // vtable. This object is disposable: we must release it to avoid hidden
    // reference counts.
    //
    unique_ptr<IDirect3D9, void (*) (IDirect3D9*)> d3d9_interface (
      reinterpret_cast<IDirect3D9*(WINAPI*) (UINT)>
        (d3d9_create) (D3D_SDK_VERSION),
      [] (IDirect3D9* p)
    {
      if (p)
        p->Release ();
    });

    // The interface creation may fail for a number of reasons: corrupted
    // runtime, missing driver, exhausted system resources, or (improbably) a
    // GPU that never learned to speak Direct3D9 (on x64 no less). In all cases
    // there is nothing sensible we can do to recover, so fail early and
    // clearly.
    //
    if (d3d9_interface == nullptr)
      throw runtime_error ("unable to create IDirect3D9 interface");

    // At this stage we extract the pointer to the first entry of the Direct3D9
    // vtable. This gives us the raw COM function table as dictated by the
    // Windows ABI. While the C++ standard disapproves of such reinterpretation
    // (technically undefined in ISO terms), the COM contract has been carved in
    // stone for decades and every relevant compiler abides by it.
    //
    // Layout reminder:
    //
    //   [object] +0x00 -> vtable pointer (array of function pointers)
    //
    // In practice, any COM interface pointer is just an object whose first and
    // only data member is the vtable pointer. Since COM interfaces are
    // pure-virtual and never multiply-inherited, this assumption holds across
    // compilers and optimization levels.
    //
    void** d3d9_vtable (*reinterpret_cast<void***> (d3d9_interface.get()));

    // In the early days, life was simple: the Import Address Table and other
    // function pointer tables were mapped read/write by default. If you wished
    // to redirect a function call, you simply wrote over the entry and called
    // it a day. That simplicity, of course, also made it trivial for malware to
    // do the same, and so modern Windows protects these tables by default.
    //
    // The direct consequence of course is that any attempt to patch a vtable
    // now requires a small dance: first request temporary write access with
    // VirtualProtect, then perform the substitution, and finally restore the
    // original protection. Failed that, an innocent assignment becomes an
    // access violation, and our hook never lands.
    //
    // Note that our method assumes the vtable we patch is stable for the
    // lifetime of the module. That assumption breaks when delay-loaders are in
    // play. Delay-loading defers the resolution of imports until first use and
    // may briefly replace or re-protect the memory region containing those
    // tables when the runtime finishes its work.
    //
    // If a delay-loader touches the same pages after we have restored their
    // original protections, the loader's subsequent write/relocation activity
    // can race with our restored protections and produce an access violation.
    // In other words, restoring the page permissions is normally the correct,
    // polite thing to do, but if a delay-loader later expects to write the same
    // memory, the polite thing becomes a fault.
    //
    // Tools sometimes suggested to force import resolution (for example
    // __HrLoadAllImportsForDll) are not portable: on MinGW that symbol isn't
    // available, so we cannot rely on it as a universal fix. In practice this
    // situation is rare, it only arises if we proxy a module that itself uses
    // delay-loading (for example proxying d3d9) or if we resolve imports
    // outside the normal loader (for example by running the target through our
    // own PE loader).
    //
    // For now the current strategy is to adopt a best-effort posture. If we
    // ever receive bug reports describing access violations that point at this
    // race, then we'll have to revisit our approach (for example by
    // coordinating resolution or deferring protection restoration in scenarios
    // we can control).
    //
    // See: mingw-w64 discussions on import-resolution behavior for background.
    //
    unsigned long o (0);
    if (VirtualProtect (reinterpret_cast<void*> (&d3d9_vtable [16]),
                        sizeof (void*),
                        PAGE_EXECUTE_READWRITE,
                        &o) != 0)
    {
      create_device = reinterpret_cast<create_device_t> (d3d9_vtable [16]);

      // Save the original CreateDevice entry and install our hook in its place.
      // From this point on, all device creation requests will come through us.
      //
      d3d9_vtable [16] = reinterpret_cast<void*> (
        +[] (IDirect3D9* d3d9,
             UINT adapter,
             D3DDEVTYPE device_type,
             HWND focus_window,
             DWORD behavior_flags,
             D3DPRESENT_PARAMETERS* presentation_parameters,
             IDirect3DDevice9** returned_device_interface) -> HRESULT
      {
        // Call the original CreateDevice first. Only on success do we have a
        // valid device pointer and thus a chance to patch EndScene.
        //
        HRESULT r (create_device (d3d9,
                                  adapter,
                                  device_type,
                                  focus_window,
                                  behavior_flags,
                                  presentation_parameters,
                                  returned_device_interface));

        if (SUCCEEDED (r))
        {
          // Extract the device vtable. Same COM layout as before: object points
          // to a single-member vtable pointer.
          //
          void** device_vtable (
            *reinterpret_cast<void***> (*returned_device_interface));

          // Again, relax protection, patch the entry, and restore. This routine
          // is essentially the same surgery we performed on CreateDevice.
          //
          unsigned long op (0);
          if (VirtualProtect (reinterpret_cast<void*> (&device_vtable [42]),
                              sizeof (void*),
                              PAGE_EXECUTE_READWRITE,
                              &op) != 0)
          {
            endscene = reinterpret_cast<endscene_t> (device_vtable [42]);

            // Save the original EndScene function so we can call it later, then
            // overwrite the vtable entry with our hook.
            //
            device_vtable [42] =
              reinterpret_cast<void*> (+[] (LPDIRECT3DDEVICE9 device) -> HRESULT
            {
              // This is where we can intercept every frame render. For now we
              // only pass control to the original EndScene, but hooks can be
              // added here.
              //
              notify_frame_render (device);

              return endscene (device);
            });

            // Restore original page protection after patching EndScene.
            //
            if (VirtualProtect (reinterpret_cast<void*> (&device_vtable [42]),
                                sizeof (void*),
                                op,
                                &op) == 0)
            {
              unsigned long e (GetLastError ());

              cerr << "warning: unable to restore page protection for EndScene: "
                   << format_message (e) << endl;
            }
          }
          else
          {
            // If we fail here, the device is created but EndScene is untouched.
            //
            ostringstream s;
            unsigned long e (GetLastError ());

            s << "unable to change page protection for EndScene: "
              << format_message (e) << endl;

            throw runtime_error (s.str ());
          }
        }

        return r;
      });

      // Finally, restore the original protection for the CreateDevice entry.
      //
      if (VirtualProtect (reinterpret_cast<void*> (&d3d9_vtable [16]),
                          sizeof (void*),
                          o,
                          &o) == 0)
      {
        unsigned long e (GetLastError ());

        cerr << "warning: unable to restore page protection for CreateDevice: "
             << format_message (e) << endl;
      }
    }
    else
    {
      // If we cannot modify the vtable at all, then nothing else can proceed.
      //
      ostringstream s;
      unsigned long e (GetLastError ());

      s << "error: unable to change page protection for CreateDevice: "
        << format_message (e) << endl;

      throw runtime_error (s.str ());
    }

    // Set this instance as the current renderer so that it may
    // participate in frame notifications.
    //
    instance = this;
  }

  renderer::signal &
  renderer::on_frame () noexcept
  {
    return frame_signal;
  }

  void
  notify_frame_render (IDirect3DDevice9 *device)
  {
    assert (device);

    // Fire the signal to notify all connected slots
    //
    try
    {
      instance->frame_signal (device);
    }
    catch (const exception &e)
    {
      cerr << "error: exception during frame render notification: "
           << e.what () << endl;

      terminate ();
    }
  }
}
