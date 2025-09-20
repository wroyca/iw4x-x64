#pragma once

#include <iosfwd>
#include <string>

#include <libiw4x/export.hxx>

namespace iw4x
{
  // Print a greeting for the specified name into the specified
  // stream. Throw std::invalid_argument if the name is empty.
  //
  LIBIW4X_SYMEXPORT void
  say_hello (std::ostream&, const std::string& name);
}
