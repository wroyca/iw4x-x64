#include <libiw4x/mod/oob.hxx>

#include <array>
#include <cassert>
#include <iostream>
#include <utility>

#include <libiw4x/utility/utility-win32.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  namespace mod
  {
    namespace
    {
      scheduler* sched (nullptr);
      oob* oob_c (nullptr);
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
      void
      oob_dispatch_stub ();
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

        memmove (reinterpret_cast<void*> (t), sequence.data (), sequence.size ());
      }
    }
  }
}
