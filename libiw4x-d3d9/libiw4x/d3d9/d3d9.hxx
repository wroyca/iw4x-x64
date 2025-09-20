#pragma once

#include <iosfwd>
#include <string>

#include <libiw4x/d3d9/export.hxx>

namespace iw4x
{
  namespace d3d9
  {
    // Print a greeting for the specified name into the specified
    // stream. Throw std::invalid_argument if the name is empty.
    //
    LIBIW4X_D3D9_SYMEXPORT void
    say_hello (std::ostream&, const std::string& name);
  }
}
