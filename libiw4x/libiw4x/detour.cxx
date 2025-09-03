#include <libiw4x/detour.hxx>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace std;

namespace iw4x
{
  namespace detour
  {
    ostream &
    operator<< (ostream &os, const exception &e)
    {
      e.print (os);
      return os;
    }

    transaction_error::
    transaction_error (const string& m)
      : message_ (m)
    {
    }

    const string &transaction_error::
    message () const noexcept
    {
      return message_;
    }

    void transaction_error::
    print (ostream &os) const
    {
      os << "transaction error '" << message ().c_str () << "'";
    }

    const char *transaction_error::
    what () const noexcept
    {
      return message_.c_str ();
    }

    zydis_error::
    zydis_error (const string& m)
      : message_ (m),
        status_ (ZYAN_STATUS_SUCCESS)
    {
    }

    zydis_error::
    zydis_error (const string& m, ZyanStatus s)
      : message_ (m),
        status_ (s)
    {
    }

    const string &zydis_error::
    message () const noexcept
    {
      return message_;
    }

    ZyanStatus zydis_error::
    status () const noexcept
    {
      return status_;
    }

    void zydis_error::
    print (ostream &os) const
    {
      os << "zydis error '" << message ().c_str ();
      if (status_ != ZYAN_STATUS_SUCCESS)
        os << " (status: " << std::hex << status_ << std::dec << ")";
      os << "'";
    }

    const char *zydis_error::
    what () const noexcept
    {
      return message_.c_str ();
    }

    interception_error::
    interception_error (const string& m)
      : message_ (m),
        error_code_ (0)
    {
    }

    interception_error::
    interception_error (const string& m,
                        DWORD ec)
      : message_ (m),
        error_code_ (ec)
    {
    }

    const string &interception_error::
    message () const noexcept
    {
      return message_;
    }

    DWORD interception_error::
    error_code () const noexcept
    {
      return error_code_;
    }

    void interception_error::
    print (ostream &os) const
    {
      os << "interception error'" << message ().c_str ();
      if (error_code_ != 0)
        os << " (" << error_code_ << ")";
      os << "'";
    }

    const char *interception_error::
    what () const noexcept
    {
      return message_.c_str ();
    }

    disassembler::
    disassembler ()
    {
      ZyanStatus r (ZydisDecoderInit (&decoder_,
                                      ZYDIS_MACHINE_MODE_LONG_64,
                                      ZYDIS_STACK_WIDTH_64));

      if (!ZYAN_SUCCESS (r))
        throw zydis_error ("unable to initialize decoder", r);

      r = ZydisFormatterInit (&formatter_, ZYDIS_FORMATTER_STYLE_INTEL);

      if (!ZYAN_SUCCESS (r))
        throw zydis_error ("unable to initialize formatter", r);
    }

    bool disassembler::
    decode (void *a, ZydisDecodedInstruction &di) const
    {
      ZydisDecodedOperand operands [ZYDIS_MAX_OPERAND_COUNT];

      ZyanStatus r (ZydisDecoderDecodeFull (&decoder_,
                                            a,
                                            16,
                                            &di,
                                            operands));

      if (!ZYAN_SUCCESS (r))
      {
        cerr << "warning: unable to decode instruction at " << a << endl;
        return false;
      }

      return true;
    }

    size_t disassembler::
    length (void *address) const
    {
      ZydisDecodedInstruction instruction;

      if (!decode (address, instruction))
        throw zydis_error ("unable to determine instruction length");

      return instruction.length;
    }

    bool disassembler::
    format (const ZydisDecodedInstruction &di,
            void *a,
            char *b,
            size_t bs) const
    {
      // Need to decode again to get operands for formatting.
      //
      ZydisDecodedOperand operands [ZYDIS_MAX_OPERAND_COUNT];

      ZyanStatus r (ZydisDecoderDecodeFull (&decoder_,
                                            a,
                                            16,
                                            const_cast<ZydisDecodedInstruction *> (&di),
                                            operands));

      if (!ZYAN_SUCCESS (r))
      {
        cerr << "warning: unable to re-decode instruction for formatting" << endl;
        return false;
      }

      r = ZydisFormatterFormatInstruction (&formatter_,
                                           &di,
                                           operands,
                                           di.operand_count_visible,
                                           b,
                                           bs,
                                           reinterpret_cast<ZyanU64> (a),
                                           ZYAN_NULL);

      if (!ZYAN_SUCCESS (r))
      {
        cerr << "warning: unable to format instruction" << endl;
        return false;
      }

      return true;
    }

    struct transaction::operation
    {
      enum class type
      {
        attach,
        detach
      };

      type type_;
      void *target;
      void *hook;
      void **original;

      operation (enum type t,
                 void *target_ptr,
                 void *hook_ptr = nullptr,
                 void **orig = nullptr)
        : type_ (t),
          target (target_ptr),
          hook (hook_ptr),
          original (orig)
      {
      }
    };

    transaction::
    transaction ()
      : state_ (state::preparing)
    {
    }

    void transaction::
    attach (void *target, void *hook, void **original)
    {
      if (state_ != state::preparing)
        throw transaction_error ("cannot attach to committed transaction");

      if (target == nullptr)
        throw transaction_error ("target cannot be null");

      if (hook == nullptr)
        throw transaction_error ("hook cannot be null");

      // Check for duplicate targets.
      //
      for (const operation &op : operations_)
      {
        if (op.target == target)
        {
          cerr << "warning: target " << target << " already exists in transaction" << endl;
          return;
        }
      }

      operations_.emplace_back (operation::type::attach,
                                target,
                                hook,
                                original);
    }

    void transaction::
    detach (void *target)
    {
      if (state_ != state::preparing)
        throw transaction_error ("cannot detach from committed transaction");

      if (target == nullptr)
        throw transaction_error ("target cannot be null");

      operations_.emplace_back (operation::type::detach, target);
    }

    void transaction::
    commit ()
    {
      if (state_ != state::preparing)
        throw transaction_error ("transaction already committed");

      if (operations_.empty ())
      {
        state_ = state::committed;
        return;
      }

      lock_guard<mutex> lock (details::m);

      for (const operation &op : operations_)
      {
        switch (op.type_)
        {
          case operation::type::attach: break;
          case operation::type::detach: break;
        }
      }

      state_ = state::committed;
    }

    void transaction::
    rollback ()
    {
      if (state_ == state::preparing)
      {
        operations_.clear ();
      }
      else if (state_ == state::committed)
      {
        lock_guard<mutex> lock (details::m);

        for (auto it (operations_.rbegin ()); it != operations_.rend (); ++it)
        {
          switch (it->type_)
          {
            case operation::type::attach: break;
            case operation::type::detach: break;
          }
        }
      }

      operations_.clear ();
      state_ = state::preparing;
    }

    bool transaction::
    empty () const noexcept
    {
      return operations_.empty ();
    }

    size_t transaction::
    size () const noexcept
    {
      return operations_.size ();
    }

    void transaction::
    clear ()
    {
      if (state_ != state::preparing)
      {
        cerr << "warning: cannot clear committed transaction" << endl;
        return;
      }

      operations_.clear ();
    }
  }
}
