#pragma once

#include <exception>
#include <ostream>

#include <libiw4x/export.hxx>

namespace iw4x
{
  namespace detour
  {
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
      transaction_error (const std::string &message);

      virtual ~transaction_error () noexcept;

      const std::string &
      message () const;

      virtual void
      print (std::ostream &) const;

      virtual const char *
      what () const noexcept;

    private:
      std::string message_;
    };
  }
}
