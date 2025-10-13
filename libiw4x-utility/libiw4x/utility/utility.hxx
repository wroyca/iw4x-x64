#pragma once

#include <iosfwd>
#include <string>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  namespace utility
  {
    // Print a greeting for the specified name into the specified
    // stream. Throw std::invalid_argument if the name is empty.
    //
    LIBIW4X_UTILITY_SYMEXPORT void
    say_hello (std::ostream&, const std::string& name);
  }
}
