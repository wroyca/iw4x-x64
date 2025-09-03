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
  }
}
