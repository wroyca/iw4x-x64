#include <libiw4x/iw4x.hxx>

#include <array>
#include <cstddef>
#include <iostream>


#include <libiw4x/console-win32.hxx>
#include <libiw4x/imgui.hxx>


using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  extern "C"
  {
    // DLL entry point. Executes while the process loader lock is held.
    //
    BOOL WINAPI
    DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    {
      if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

      attach_console ();

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
      // The patch itself is just a manual memory write. This is by design: the
      // less we do in a DLL entry point, the better. Introducing dynamic
      // behavior here would only enlarge the set of things that can go wrong.
      //
      // Note also that our executable's startup routine is in actually Windows
      // CRT startup which relies on a pair of well-known routines to initialize
      // it internal state: namely, __security_init_cookie (which sets up the
      // stack cookie for buffer overrun protection) and __scrt_common_main_seh
      // (which performs the actual C runtime startup).
      //
      // These two routines are part of the compiler-provided runtime and are
      // treated as opaque internals of the toolchain. For this reason, we
      // deliberately avoid creating delegate types or symbolic wrappers for
      // them, unlike the rest of our system-level patch points. They are called
      // directly via hardcoded addresses with explicit reinterpret_casts, as
      // these are transitional control transfers into non-user code, not
      // application-level function references.
      //
      // This distinction is intentional: we do not expect readers or
      // maintainers to interact with or override these symbols; they represent
      // fixed CRT behavior. That is, we avoid symbolic abstraction to signal
      // that these calls are special-case bootstrap mechanisms outside the
      // application's purview.
      //
      uintptr_t target (0x140358EBC);
      uintptr_t source (reinterpret_cast<decltype (source)> (+[] ()
      {
        // The CRT startup sequence normally begins by seeding its own internal
        // security cookie. This is the value checked by /GS instrumentation to
        // detect stack corruption. We must call `__security_init_cookie()`
        // ourselves here to preserve the compiler's expected invariants.
        //
        reinterpret_cast<void (*) ()> (0x1403598CC) ();

        // Perform IW4x-specific initialization before transferring control
        // to the original C runtime startup. See DllMain() for context.
        //
        ([] (auto&& _)
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

        imgui imgui;

        // After the cookie has been seeded, the standard CRT entry point would
        // transfer control to `__scrt_common_main_seh()`.
        //
        return reinterpret_cast<int (*) ()> (0x140358D48) ();
      }));

      // Encoding a 64-bit absolute jump:
      //
      // - FF 25 00000000   ; JMP QWORD PTR [RIP + 0]
      // - <64-bit address> ; Absolute destination in little-endian
      //
      // On x86-64, the `jmp` instruction does not provide an immediate
      // absolute form. Instead, the canonical way to transfer control to a
      // 64-bit address is via an indirect jump through memory. The form
      // `FF /4` encodes `jmp r/m64`, and when paired with a RIP-relative
      // addressing mode, it allows embedding the destination address as a
      // literal directly after the instruction.
      //
      // With a displacement of zero, the effective address becomes
      // "RIP + 0", that is, the address immediately following the
      // instruction. The processor fetches the 64-bit value stored there and
      // uses it as the jump target.
      //
      // The resulting sequence is 14 bytes total: six bytes for the opcode
      // and displacement, followed by the eight-byte absolute address.
      //
      array<std::byte, 14> sequence
      {
        {
          static_cast<std::byte> (0xFF),
          static_cast<std::byte> (0x25),
          static_cast<std::byte> (0x00),
          static_cast<std::byte> (0x00),
          static_cast<std::byte> (0x00),
          static_cast<std::byte> (0x00),
          static_cast<std::byte> ((source)       & 0xFF),
          static_cast<std::byte> ((source >> 8)  & 0xFF),
          static_cast<std::byte> ((source >> 16) & 0xFF),
          static_cast<std::byte> ((source >> 24) & 0xFF),
          static_cast<std::byte> ((source >> 32) & 0xFF),
          static_cast<std::byte> ((source >> 40) & 0xFF),
          static_cast<std::byte> ((source >> 48) & 0xFF),
          static_cast<std::byte> ((source >> 56) & 0xFF)
        }
      };

      DWORD o (0);

      if (VirtualProtect (reinterpret_cast<LPVOID> (target),
                          sequence.size (),
                          PAGE_EXECUTE_READWRITE,
                          &o) == 0)
      {
        cerr << "error: unable to change page protection at address "
             << hex << target << dec << endl;

        return FALSE;
      }

      if (memcpy (reinterpret_cast<void *> (target),
                  sequence.data (),
                  sequence.size ()) == nullptr)
      {
        cerr << "error: unable to copy instruction sequence to target address "
             << hex << target << dec << "\n";

        return FALSE;
      }

      if (VirtualProtect (reinterpret_cast<LPVOID> (target),
                          sequence.size (),
                          o,
                          &o) == 0)
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
