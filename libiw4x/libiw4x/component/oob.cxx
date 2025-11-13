#include <libiw4x/component/oob.hxx>

#include <cassert>
#include <iostream>
#include <utility>

#include <libiw4x/utility/utility-win32.hxx>
#include <libiw4x/types.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  namespace
  {
    scheduler* sched (nullptr);
    oob*       oob_c (nullptr);
  }

  extern "C"
  {
    bool
    oob_dispatch (const netadr_t* a, const msg_t* m)
    {
      try
      {
        return oob_c->dispatch (*a, *m);
      }
      catch (const exception& e)
      {
        cerr << "error: " << e.what () << endl;
        exit (1);
      }
    }

    // OOB packet interception (CL_DispatchConnectionlessPacket)
    //
    __attribute__ ((naked)) void
    oob_dispatch_stub ()
    {
      __asm__ __volatile__
      (
        // Save return address.
        //
        "push %rax\n\t"
        "mov 0x8(%rsp), %rax\n\t"
        "mov %rax, -0x8(%rsp)\n\t"
        "pop %rax\n\t"

        // Save all registers.
        //
        "push %rax\n\t"
        "push %rcx\n\t"
        "push %rdx\n\t"
        "push %rbx\n\t"
        "push %rbp\n\t"
        "push %rsi\n\t"
        "push %rdi\n\t"
        "push %r8\n\t"
        "push %r9\n\t"
        "push %r10\n\t"
        "push %r11\n\t"
        "push %r12\n\t"
        "push %r13\n\t"
        "push %r14\n\t"
        "push %r15\n\t"

        // Allocate shadow space and align stack.
        //
        "sub $0x28, %rsp\n\t"

        // Setup parameters: RCX = RBX (addr), RDX = R15 (msg).
        //
        "mov %rbx, %rcx\n\t"
        "mov %r15, %rdx\n\t"

        // Call C wrapper.
        //
        "call oob_dispatch\n\t"

        // Save result.
        //
        "mov %al, %cl\n\t"

        // Restore stack.
        //
        "add $0x28, %rsp\n\t"

        // Restore all registers.
        //
        "pop %r15\n\t"
        "pop %r14\n\t"
        "pop %r13\n\t"
        "pop %r12\n\t"
        "pop %r11\n\t"
        "pop %r10\n\t"
        "pop %r9\n\t"
        "pop %r8\n\t"
        "pop %rdi\n\t"
        "pop %rsi\n\t"
        "pop %rbp\n\t"
        "pop %rbx\n\t"
        "pop %rdx\n\t"
        "pop %rcx\n\t"
        "pop %rax\n\t"

        // Check result.
        //
        "test %cl, %cl\n\t"
        "jz 1f\n\t"

        // Handler signalled consumption. Return directly to caller.
        //
        "mov $1, %al\n\t"
        "ret\n\t"

        // Handler declined. Resume original control flow.
        //
        "1:\n\t"
        "movabs $0x1400F6065, %r11\n\t"
        "jmp *%r11\n\t"
      );
    }
  }

  bool oob::
  dispatch (const netadr_t& a, const msg_t& m)
  {
    if (m.data == nullptr || m.cursize <= 4)
      return false;

    vector<string> args;

    const char* p   (m.data + 4);
    const char* end (m.data + m.cursize);

    string token;
    bool in_quotes (false);

    for (; p < end; ++p)
    {
      char c (*p);

      // Null terminator or newline ends the command line.
      //
      if (c == '\0' || c == '\n')
        break;

      // Toggle quote state.
      //
      if (c == '"')
      {
        in_quotes = !in_quotes;
        continue;
      }

      // Outside quotes, whitespace delimits tokens.
      //
      if (!in_quotes && (c == ' ' || c == '\t' || c == '\r'))
      {
        if (!token.empty ())
        {
          args.push_back (std::move (token));
          token.clear ();
        }
        continue;
      }

      // Accumulate character into current token.
      //
      token += c;
    }

    // Add final token if present.
    //
    if (!token.empty ())
      args.push_back (std::move (token));

    // Command must be present.
    //
    if (args.empty ())
      return false;

    // Lookup and invoke handler.
    //
    const string& n (args[0]);

    unordered_map<string, handler>::const_iterator i (handlers_.find (n));
    if (i == handlers_.end ())
      return false;

    try
    {
      i->second (a, args);
      return true;
    }
    catch (const exception& e)
    {
      cerr << "error: unable to dispatch '" << n << "': " << e.what () << endl;
      exit (1);
    }
  }

  void oob::
  register_handler (const string& n, handler h)
  {
    if (n.empty ())
      throw invalid_argument ("empty name");

    handlers_[n] = std::move (h);
  }

  oob::
  oob (scheduler& s)
  {
    sched = &s;
    oob_c = this;

    // Install OOB packet interception.
    //
    // Note that we do not use MinHook here. That is, the target site
    // (CL_DispatchConnectionlessPacket) is not a conventional function
    // prologue and the dispatch path requires full control over the call
    // frame.
    //
    {
      uintptr_t t (0x1400F6040);
      uintptr_t h (reinterpret_cast<uintptr_t> (&oob_dispatch_stub));

      // FF 25 00 00 00 00
      // <8-byte address>
      //
      array<unsigned char, 14> sequence
      {
        static_cast<unsigned char> (0xFF),
        static_cast<unsigned char> (0x25),
        static_cast<unsigned char> (0x00),
        static_cast<unsigned char> (0x00),
        static_cast<unsigned char> (0x00),
        static_cast<unsigned char> (0x00),
        static_cast<unsigned char> (h       & 0xFF),
        static_cast<unsigned char> (h >>  8 & 0xFF),
        static_cast<unsigned char> (h >> 16 & 0xFF),
        static_cast<unsigned char> (h >> 24 & 0xFF),
        static_cast<unsigned char> (h >> 32 & 0xFF),
        static_cast<unsigned char> (h >> 40 & 0xFF),
        static_cast<unsigned char> (h >> 48 & 0xFF),
        static_cast<unsigned char> (h >> 56 & 0xFF)
      };

      DWORD o (0);

      if (VirtualProtect (reinterpret_cast<LPVOID> (t),
                          sequence.size (),
                          PAGE_READWRITE,
                          &o) == 0)
      {
        cerr << "error: unable to change page protection";
        exit (1);
      }

      if (memcpy (reinterpret_cast<void*> (t),
                  sequence.data (),
                  sequence.size ()) == nullptr)
      {
        cerr << "error: unable to copy instruction sequence";
        exit (1);
      }

      if (VirtualProtect (reinterpret_cast<LPVOID> (t),
                          sequence.size (),
                          o,
                          &o) == 0)
      {
        cerr << "error: unable to restore page protection";
        exit (1);
      }

      if (FlushInstructionCache (GetCurrentProcess (),
                                 reinterpret_cast<LPCVOID> (t),
                                 sequence.size ()) == 0)
      {
        cerr << "error: unable to flush instruction cache";
        exit (1);
      }
    }
  }
}
