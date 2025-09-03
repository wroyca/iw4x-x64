#pragma once

#include <cstddef>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

#include <Zydis/Zydis.h>
#include <Zycore/Zycore.h>

#include <libiw4x/utility/utility-win32.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  namespace detour
  {
    namespace details
    {
      inline std::mutex m;
    }

    // Exceptions in this subsystem are represented not by raw strings or the
    // standard exception hierarchy but by a dedicated interface. The rationale
    // is that detour-specific errors typically carry richer semantics than a
    // diagnostic message. In particular, many error conditions are best
    // understood when formatted with context (e.g., which symbol failed to
    // patch, what range was invalid, which system call returned an unexpected
    // result).
    //
    class LIBIW4X_SYMEXPORT exception : public std::exception
    {
    public:
      // Each concrete exception must be able to describe itself in human-
      // readable form. The `print()` contract is that the method should write a
      // complete, self-contained diagnostic message into the supplied stream
      // without throwing.
      //
      virtual void
      print (std::ostream &) const = 0;
    };

    // Stream insertion operator
    //
    // Provides uniform syntax for emitting detour exceptions. Internally this
    // simply delegates to `print()`, thereby ensuring that any user-defined
    // exception type has its own formatting logic applied consistently. Note
    // that we return the stream to preserve operator chaining semantics.
    //
    std::ostream&
    operator<< (std::ostream&, const exception&);

    // transaction_error
    //
    // While `detour::exception` defines the abstract contract, there are cases
    // where a full hierarchy of specialized exception types would be excessive.
    // Many operations in this subsystem fail in ways that can be adequately
    // summarized with a textual message. For such cases, we provide
    // `transaction_error`, a concrete implementation that carries only a
    // string.
    //
    // The design intention is pragmatic: when a transaction fails, the failure
    // context is usually reducible to "what we tried to do" and "why it could
    // not be done". A plain string is sufficient for this, and introducing a
    // proliferation of subclasses would not improve diagnosability. Thus
    // `transaction_error` serves as the general-purpose, message-bearing error
    // type.
    //
    class LIBIW4X_SYMEXPORT transaction_error : public exception
    {
    public:
      explicit
      transaction_error (const std::string& message);

      const std::string &
      message () const noexcept;

      virtual void
      print (std::ostream &) const;

      virtual const char *
      what () const noexcept;

    private:
      std::string message_;
    };

    class LIBIW4X_SYMEXPORT zydis_error : public exception
    {
    public:
      explicit
      zydis_error (const std::string &);

      explicit
      zydis_error (const std::string &, ZyanStatus status);

      const std::string &
      message () const noexcept;

      ZyanStatus
      status () const noexcept;

      virtual void
      print (std::ostream &) const override;

      virtual const char *
      what () const noexcept override;

    private:
      std::string message_;
      ZyanStatus status_;
    };

    class LIBIW4X_SYMEXPORT interception_error : public exception
    {
    public:
      explicit
      interception_error (const std::string &message);

      explicit
      interception_error (const std::string &message, DWORD error_code);

      const std::string &
      message () const noexcept;

      DWORD
      error_code () const noexcept;

      virtual void
      print (std::ostream &) const override;

      virtual const char *
      what () const noexcept override;

    private:
      std::string message_;
      DWORD error_code_;
    };

    class LIBIW4X_SYMEXPORT disassembler
    {
    public:
      disassembler ();

      // Decode instruction at address.
      //
      bool
      decode (void *address, ZydisDecodedInstruction &instruction) const;

      // Get instruction length at address.
      //
      std::size_t
      length (void *address) const;

      // Format instruction for display.
      //
      bool
      format (const ZydisDecodedInstruction &instruction,
              void *address,
              char *buffer,
              std::size_t buffer_size) const;

    private:
      ZydisDecoder decoder_;
      ZydisFormatter formatter_;
    };

    class LIBIW4X_SYMEXPORT transaction
    {
    public:
      enum class state
      {
        preparing,
        committed
      };

      transaction ();

      // Prepare a detour for the specified target function.
      //
      void
      attach (void *target, void *hook, void **original = nullptr);

      // Prepare removal of a detour for the specified target function.
      //
      void
      detach (void *target);

      // Atomically apply all prepared operations.
      //
      void
      commit ();

      // Rollback all operations in this transaction.
      //
      void
      rollback ();

      // Check if the transaction has pending operations.
      //
      bool
      empty () const noexcept;

      // Get the number of pending operations.
      //
      std::size_t
      size () const noexcept;

      // Clear all pending operations without applying them.
      //
      void
      clear ();

    private:
      state state_;

      // Individual action performed within the transaction
      //
      struct operation;
      std::vector<operation> operations_;
    };
  }
}
