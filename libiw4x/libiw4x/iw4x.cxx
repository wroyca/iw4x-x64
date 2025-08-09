#include <libiw4x/iw4x.hxx>

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <thread>
#include <chrono>

#include <libiw4x/console-win32.hxx>

using namespace std;

namespace iw4x
{
  namespace
  {
    int
    start ()
    {
      // __security_init_cookie
      //
      reinterpret_cast<void (*) ()> (0x1403598CC) ();

      // Perform IW4x-specific initialization before transferring control
      // to the original C runtime startup. See DllMain() for context.
      //
      ([] (auto&& _)
      {
        _(0x1401B2FD2, 0x88, 1); // Skip xgameruntime.dll
        _(0x1401B3096, 0x78, 1); // Ignore XGameRuntimeInitialize failure

        _(0x1402A6A4B, 0x90, 5); // NOP out CurlX initialization
        _(0x1402A6368, 0x90, 5); // NOP out CurlX cleanup

        _(0x1402A5F70, 0x90, 3); // Skip flag clobbering
        _(0x1402A5F73, 0x74, 1); // Bypass Xbox Live restriction
        _(0x1400F5B86, 0xEB, 1); // Skip XBOXLIVE_SIGNINCHANGED
        _(0x1400F5BAC, 0xEB, 1); // Skip XBOXLIVE_SIGNEDOUT
        _(0x14010B332, 0xEB, 1); // Bypass Xbox Live permission
        _(0x1401BA1FE, 0xEB, 1); // Always pass signed-in status

        _(0x140271ED0, 0xC3, 1); // Return immediately from popup creation function (disables popups)

        _(0x1400F6BC4, 0x90, 2); // Skip playlist download check
        _(0x1400FC833, 0xEB, 1); // Skip config string mismatch (1)
        _(0x1400D2AFC, 0x90, 2); // Skip config string mismatch (2)

        _(0x1400E4DA0, 0x33, 1); // Skip crash from stats
        _(0x1400E4DA1, 0xC0, 1); //
        _(0x1400E4DA2, 0xC3, 1); //
      })

      ([] (uintptr_t a, int v, size_t s)
      {
        DWORD o (0);
        void* p (reinterpret_cast<void*> (a));

        VirtualProtect (p, s, PAGE_EXECUTE_READWRITE, &o);
        memset (p, v, s);
        VirtualProtect (p, s, o, &o);
        FlushInstructionCache (GetCurrentProcess (), p, s);
      });

      console console;

      console.register_command ("iw4x", [&console] ()
      {
        console.execute_command ("downloadplaylist");
        console.execute_command ("xblive_privatematch 1");
        console.execute_command ("onlinegame 0");
        console.execute_command ("xblive_hostingprivateparty 1");
        console.execute_command ("xblive_privatepartyclient 1");
      });

      // __scrt_common_main_seh
      //
      return reinterpret_cast<int (*) ()> (0x140358D48) ();
    }
  }

  extern "C"
  {
    // DLL entry point. Executes while the process loader lock is held.
    //
    BOOL WINAPI
    DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    {
      if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

#if LIBIW4X_DEVELOP
      // IW4 was linked with /SUBSYSTEM:WINDOWS which implies to the operating
      // system that a console is not needed. In that instance, the standard
      // handles retrieved with GetStdHandle will likely be invalid on startup
      // until AttachConsole is called.
      //
      if (AttachConsole (ATTACH_PARENT_PROCESS) != 0)
      {
        FILE *in = nullptr, *out = nullptr, *err = nullptr;

        // If this fails, there is little we can do. Treat this as best-effort
        // redirection and suppress all errors unconditionally.
        //
        (void) freopen_s (&in,  "CONIN$",  "r", stdin);
        (void) freopen_s (&out, "CONOUT$", "w", stdout);
        (void) freopen_s (&err, "CONOUT$", "w", stderr);
      }
#endif

      // Note that any attempt to acquire additional locks or perform complex
      // operations risks violating internal ordering guarantees. The strategy
      // here is to patch IW4's startup routine to execute our code outside the
      // constraints imposed by the loader lock.
      //
      uintptr_t target (0x140358EBC);
      uintptr_t source (reinterpret_cast<decltype(source)> (&start));

      // Encoding:
      //
      //   FF 25 00000000 ; JMP QWORD PTR [RIP + 0] <64-bit address>
      //   Absolute destination (little-endian)
      //
      // RIP-relative memory operands are computed relative to the next
      // instruction. The zero displacement causes the jump to read the address
      // literal immediately following the instruction.
      //
      array<byte, 14> sequence {{static_cast<byte> (0xFF),
                                 static_cast<byte> (0x25),
                                 static_cast<byte> (0x00),
                                 static_cast<byte> (0x00),
                                 static_cast<byte> (0x00),
                                 static_cast<byte> (0x00),
                                 static_cast<byte> ((source) & 0xFF),
                                 static_cast<byte> ((source >> 8) & 0xFF),
                                 static_cast<byte> ((source >> 16) & 0xFF),
                                 static_cast<byte> ((source >> 24) & 0xFF),
                                 static_cast<byte> ((source >> 32) & 0xFF),
                                 static_cast<byte> ((source >> 40) & 0xFF),
                                 static_cast<byte> ((source >> 48) & 0xFF),
                                 static_cast<byte> ((source >> 56) & 0xFF)}};

      DWORD old_protect (0);

      if (VirtualProtect (reinterpret_cast<LPVOID> (target),
                          sequence.size (),
                          PAGE_EXECUTE_READWRITE,
                          &old_protect) == 0)
      {
        cerr << "error: unable to change page protection at address "
             << hex << target << dec << "\n";

        return FALSE;
      }

      if (memcpy (reinterpret_cast<void *> (target),
                  sequence.data (),
                  sequence.size ()) == nullptr)
      {
        cerr << "error: unable to copy jump instruction to target address "
             << hex << target << dec << "\n";

        return FALSE;
      }

      if (VirtualProtect (reinterpret_cast<LPVOID> (target),
                          sequence.size (),
                          old_protect,
                          &old_protect) == 0)
      {
        cerr << "warning: unable to restore page protection at address "
             << hex << target << dec << "\n";
      }

      if (FlushInstructionCache (GetCurrentProcess (),
                                 reinterpret_cast<LPCVOID> (target),
                                 sequence.size ()) == 0)
      {
        cerr << "warning: unable to flush instruction cache at address "
             << hex << target << dec << "\n";
      }

      // Successful DLL_PROCESS_ATTACH.
      //
      return TRUE;
    }
  }
}
