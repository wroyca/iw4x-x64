#include <libiw4x/iw4x.hxx>

#include <iostream>
#include <array>

extern "C"
{
  #include <io.h>
}

#include <libiw4x/console-win32.hxx>

#include <libiw4x/component/frame.hxx>
#include <libiw4x/component/menu.hxx>
#include <libiw4x/component/network.hxx>
#include <libiw4x/component/oob.hxx>
#include <libiw4x/component/party.hxx>

#include <libiw4x/utility/cstring.hxx>
#include <libiw4x/utility/minhook/hook.hxx>
#include <libiw4x/utility/scheduler.hxx>

#include <libiw4x/windows/process-threads-api.hxx>

using namespace std;
using namespace iw4x::utility;
using namespace iw4x::windows;

namespace iw4x
{
  namespace
  {
    void
    attach_console ()
    {
      // The subtlety here is that Windows has many ways to end up with stdout
      // and stderr pointing *somewhere* (sometimes to an actual console,
      // sometimes to a pipe, sometimes to a completely invalid handle). We do
      // not want to attach to `CONOUT$` in cases where the existing handles are
      // already valid and intentional, as this would silently discard the real
      // output sink.
      //
      // Instead, we first check `_fileno(stdout)` and `_fileno(stderr)`. That
      // is, MSVCRT sets these up at startup and will return `-2`
      // (`_NO_CONSOLE_FILENO`) if the file is invalid. This check is more
      // trustworthy than calling `GetStdHandle()`, which can return stale
      // handle IDs that may already be reused for unrelated objects by the time
      // we run.
      //
      if (_fileno (stdout) >= 0 || _fileno (stderr) >= 0)
      {
        // If either `_fileno()` is valid, we go one step further: `_fileno()`
        // itself had a bug (http://crbug.com/358267) in SUBSYSTEM:WINDOWS
        // builds for certain MSVC versions (VS2010 to VS2013), so we
        // double-check by calling `_get_osfhandle()` to confirm that the
        // underlying OS handle is valid. Only if both streams are invalid do
        // we attempt to attach a console.
        //
        intptr_t stdout_handle (_get_osfhandle (_fileno (stdout)));
        intptr_t stderr_handle (_get_osfhandle (_fileno (stderr)));

        if (stdout_handle >= 0 || stderr_handle >= 0)
          return;
      }

      // At this point, both standard streams appear invalid, so we can safely
      // attach to the parent process's console.
      //
      if (AttachConsole (ATTACH_PARENT_PROCESS) == 0)
      {
        unsigned int error (GetLastError ());

        // `ERROR_ACCESS_DENIED` means we are already attached. Any further
        // action would be redundant and potentially unsafe.
        //
        if (error == ERROR_ACCESS_DENIED)
          return;

        // `ERROR_GEN_FAILURE` indicates that the parent console is no longer
        // usable, usually because the parent process has terminated. Creating a
        // new console in this case would only produce an orphan window.
        //
        if (error == ERROR_GEN_FAILURE)
          return;
      }

      // Once attached, rebind `stdout` and `stderr` to `CONOUT$` using
      // `freopen()`. Also duplicate their low-level descriptors (1 for stdout,
      // 2 for stderr) so that code using the raw file descriptor API observes
      // the same handles.
      //
      // Note that failure to rebind is non-fatal. Console output is
      // diagnostic-only and has no bearing on core functionality. We therefore
      // avoid exceptions and suppress all errors unconditionally.
      //
      bool stdout_rebound (false);
      bool stderr_rebound (false);

      if (freopen ("CONOUT$", "w", stdout) != nullptr &&
          _dup2 (_fileno (stdout), 1) != -1)
        stdout_rebound = true;

      if (freopen ("CONOUT$", "w", stderr) != nullptr &&
          _dup2 (_fileno (stderr), 2) != -1)
        stderr_rebound = true;

      // If stream were rebound, realign iostream objects (`cout`, `cerr`,
      // etc.) with C FILE streams.
      //
      if (stdout_rebound && stderr_rebound)
        ios::sync_with_stdio ();
    }
  }

  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    {
      if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

      // DllMain executes while the process loader lock is held.
      //
      // This is one of the few contexts where the operating system invokes our
      // code under a low-level synchronization primitive. Any attempt to
      // perform nontrivial work here risks deadlock or subtle ordering
      // violations.
      //
      // To avoid this, we do not initialize directly in DllMain. Instead, we
      // patch the executable's startup routine so that our setup runs later on
      // IW4's main thread, outside the constraints imposed by the loader lock.
      //
      uintptr_t target (0x140358EBC);
      uintptr_t source (reinterpret_cast<uintptr_t> (+[] ()
      {
        // __security_init_cookie
        //
        reinterpret_cast<void (*) ()> (0x1403598CC) ();

        // Under normal circumstances, a DLL is unloaded via FreeLibrary once
        // its reference count reaches zero. This is acceptable for auxiliary
        // libraries but unsuitable for modules like ours, which embed deeply
        // into the host process.
        //
        // To prevent FreeLibrary call on our module, whether accidental or
        // deliberate, we informs the Windows loader that our module is a
        // permanent part of the process.
        //
        HMODULE m;
        if (!GetModuleHandleEx (GET_MODULE_HANDLE_EX_FLAG_PIN |
                                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                                reinterpret_cast<LPCTSTR> (DllMain),
                                &m))
        {
          cerr << "error: unable to mark module as permanent" << endl;
          exit (1);
        }

        // By default, the process inherits its working directory from whatever
        // environment or launcher invoked it, which may vary across setups and
        // lead to unpredictable relative path resolution.
        //
        // The strategy here is to explicitly realign the working directory to
        // the DLL's own location. That is, we effectively makes all relative
        // file operations resolve against the module's directory when the
        // process is hosted or started indirectly.
        //
        // Note also that failure to resolve or change the directory is treated
        // as fatal, since continuing under an indeterminate working path would
        // likely lead to cascading file I/O errors later on.
        //
        char p [MAX_PATH];
        if (GetModuleFileName (m, p, MAX_PATH))
        {
          string s (p);
          size_t i (s.rfind ('\\'));

          if (i == string::npos ||
              !SetCurrentDirectory (s.substr (0, i).c_str ()))
          {
            cerr << "error: unable to set current directory" << endl;
            exit (1);
          }
        }
        else
        {
          cerr << "error: unable to retrieve module location" << endl;
          exit (1);
        }

        // Quick Patch
        //
        // Removes runtime dependencies on Xbox Live and XGameRuntime
        // components.
        //
        ([] (void (*_) (uintptr_t, int, size_t))
          {
            _(0x1401B2FCA, 0x31, 1); // Bypass XGameRuntimeInitialize
            _(0x1401B2FCB, 0xC0, 1); //
            _(0x1401B2FCC, 0x90, 3); //
            _(0x1401B308F, 0x31, 1); //
            _(0x1401B3090, 0xC0, 1); //
            _(0x1401B3091, 0x90, 3); //

            _(0x1402A6A4B, 0x90, 5); // NOP out CurlX initialization
            _(0x1402A6368, 0x90, 5); // NOP out CurlX cleanup

            _(0x1402A5F70, 0x90, 3); // Skip flag clobbering
            _(0x1402A5F73, 0x74, 1); // Bypass Xbox Live restriction
            _(0x1400F5B86, 0xEB, 1); // Skip XBOXLIVE_SIGNINCHANGED
            _(0x1400F5BAC, 0xEB, 1); // Skip XBOXLIVE_SIGNEDOUT
            _(0x14010B332, 0xEB, 1); // Bypass Xbox Live permission
            _(0x1401BA1FE, 0xEB, 1); // Always pass signed-in status

            _(0x140271ED0, 0xC3, 1); // Disable popup

            _(0x1400F6BC4, 0x90, 2); // Skip playlist download check
            _(0x1400FC833, 0xEB, 1); // Skip config string mismatch (1)
            _(0x1400D2AFC, 0x90, 2); // Skip config string mismatch (2)

            _(0x1400E4DA0, 0x33, 1); // Skip crash from stats
            _(0x1400E4DA1, 0xC0, 1); //
            _(0x1400E4DA2, 0xC3, 1); //
          })
        ([] (uintptr_t address, int value, size_t size)
          {
            utility::memset (address, value, size);
          });

        // Subsystem initialization
        //
        minhook::initialize ();
        process_threads_api_init ();

        scheduler scheduler;
        frame     frame (scheduler);
        menu      menu (scheduler);
        network   network (scheduler);
        oob       oob (scheduler);
        party     party (oob);
        console   console (party);

        // __scrt_common_main_seh
        //
        return reinterpret_cast<int (*) ()> (0x140358D48) ();
      }));

      // Build a 64-bit absolute jump.
      //
      // x86-64 has no single-instruction form that takes a 64-bit immediate.
      // The canonical solution is an indirect, RIP-relative jump followed by
      // the eight-byte destination in little-endian form.
      //
      // With a zero displacement the memory operand refers to the eight bytes
      // immediately after the six-byte opcode. The processor loads that value
      // and transfers control to it. The total sequence is therefore 14 bytes:
      // six for the opcode and displacement, eight for the address.
      //
      array<unsigned char, 14> sequence
      ({
        static_cast<unsigned char> (0xFF),
        static_cast<unsigned char> (0x25),
        static_cast<unsigned char> (0x00),
        static_cast<unsigned char> (0x00),
        static_cast<unsigned char> (0x00),
        static_cast<unsigned char> (0x00),
        static_cast<unsigned char> (source       & 0xFF),
        static_cast<unsigned char> (source >> 8  & 0xFF),
        static_cast<unsigned char> (source >> 16 & 0xFF),
        static_cast<unsigned char> (source >> 24 & 0xFF),
        static_cast<unsigned char> (source >> 32 & 0xFF),
        static_cast<unsigned char> (source >> 40 & 0xFF),
        static_cast<unsigned char> (source >> 48 & 0xFF),
        static_cast<unsigned char> (source >> 56 & 0xFF)
      });

      attach_console ();

      DWORD o (0);

      if (VirtualProtect (reinterpret_cast<LPVOID> (target),
                          sequence.size (),
                          PAGE_READWRITE,
                          &o) == 0)
      {
        cerr << "error: unable to change page protection";
        return FALSE;
      }

      memmove (reinterpret_cast<void*> (target),
               sequence.data (),
               sequence.size ());

      if (VirtualProtect (reinterpret_cast<LPVOID> (target),
                          sequence.size (),
                          o,
                          &o) == 0)
      {
        cerr << "error: unable to restore page protection";
        return FALSE;
      }

      if (FlushInstructionCache (GetCurrentProcess (),
                                 reinterpret_cast<LPCVOID> (target),
                                 sequence.size ()) == 0)
      {
        cerr << "error: unable to flush instruction cache";
        return FALSE;
      }

      // Successful DLL_PROCESS_ATTACH.
      //
      return TRUE;
    }
  }
}
