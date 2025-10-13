#include <libiw4x/iw4x.hxx>

#include <libiw4x/renderer.hxx>
#include <libiw4x/imgui.hxx>
#include <libiw4x/component/menu-handlers.hxx>

namespace iw4x
{
  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE instance, DWORD reason, LPVOID reserved)
    {
      if (reason != DLL_PROCESS_ATTACH)
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
      // The patch itself is just a manual memory write. This is by design: the
      // less we do in a DLL entry point, the better. Introducing dynamic
      // behavior here would only enlarge the set of things that can go wrong.
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
        if (char p [MAX_PATH]; GetModuleFileName (m, p, MAX_PATH))
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

        // Unprotect binary
        //
        MODULEINFO mi;
        if (GetModuleInformation (GetCurrentProcess (),
                                  GetModuleHandle (nullptr),
                                  &mi,
                                  sizeof (mi)))
        {
          DWORD o (0);
          if (!VirtualProtect (mi.lpBaseOfDll,
                               mi.SizeOfImage,
                               PAGE_EXECUTE_READWRITE,
                               &o))
          {
            cerr << "error: unable to change memory protection" << endl;
            exit (1);
          }
        }
        else
        {
          cerr << "error: unable to retrieve module information" << endl;
          exit (1);
        }

        // Quick Patch
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

        // Subsystem initialization
        //
        // At this stage we bring up the subsystems that are always present in
        // IW4x. You may notice there is no indirection, registration machinery,
        // or other "flexibility enablers" here.
        //
        // This is deliberate.
        //
        // Subsystem are required unconditionally, their implementations are not
        // expected to vary, and their lifetime is naturally tied to that of the
        // application lifetime. In such cases, introducing factories or dynamic
        // allocation would be without substance.
        //
        // In other words, we resist the temptation to build a "general
        // solution" where a straightforward construction suffices. Flexibility
        // can always be added later, but simplicity once lost is rarely
        // recovered.
        //
        renderer renderer;
        imgui imgui (renderer);

        // TODO: Write scheduler
        //
        CreateThread (nullptr, 0, [] (LPVOID) -> DWORD
        {
          Sleep (5000);
          menu_handlers handlers;
          return 0;
        }, nullptr, 0, nullptr);

        // Once the security cookie has been initialized and our detours
        // installed, control must be handed back to the CRT. The designated
        // entry point for this purpose is `__scrt_common_main_seh()`.
        //
        // Note that it's the mechanism by which the MSVC runtime transitions
        // from raw process state into a valid C/C++ execution environment.
        // Skipping or bypassing it would leave the process in an indeterminate
        // state, with undefined behavior on both normal execution paths and
        // during shutdown.
        //
        return reinterpret_cast<int (*) ()> (0x140358D48) ();
      }));

      // Encode our 64-bit absolute jump:
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
      array<unsigned char, 14> sequence
      {
        {
          static_cast<unsigned char> (0xFF),
          static_cast<unsigned char> (0x25),
          static_cast<unsigned char> (0x00),
          static_cast<unsigned char> (0x00),
          static_cast<unsigned char> (0x00),
          static_cast<unsigned char> (0x00),
          static_cast<unsigned char> ((source)       & 0xFF),
          static_cast<unsigned char> ((source >> 8)  & 0xFF),
          static_cast<unsigned char> ((source >> 16) & 0xFF),
          static_cast<unsigned char> ((source >> 24) & 0xFF),
          static_cast<unsigned char> ((source >> 32) & 0xFF),
          static_cast<unsigned char> ((source >> 40) & 0xFF),
          static_cast<unsigned char> ((source >> 48) & 0xFF),
          static_cast<unsigned char> ((source >> 56) & 0xFF)
        }
      };

      attach_console ();

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

      if (memcpy (reinterpret_cast<void*> (target),
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

      // On x86 and x86-64 architectures, flushing the instruction cache is
      // generally unnecessary provided that both code modification and
      // execution occur via the same linear address. In practice, it's
      // recommended to always invoke FlushInstructionCache and allow the
      // operating system to determine whether any action is required.
      //
      // For reference, see Intel(R) 64 and IA-32 Architectures Software
      // Developer's Manual, Volume 3A: System Programming Guide, Part 1,
      // Section 11.6: "Self-Modifying Code".
      //
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
