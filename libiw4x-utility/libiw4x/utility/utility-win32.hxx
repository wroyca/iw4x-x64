#pragma once

#include <libiw4x/utility/utility-win32.h>

#include <string>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  namespace utility
  {
    // Formats a Windows error code into a human-readable string message.
    //
    LIBIW4X_UTILITY_SYMEXPORT std::string
    format_message (unsigned long code);
  }
}
