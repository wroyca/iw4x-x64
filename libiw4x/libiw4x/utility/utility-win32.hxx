#pragma once

#include <memory>
#include <ostream>
#include <string>

// Include Windows.h in a way that avoids the usual namespace pollution.
//
// In particular, we temporarily define WIN32_LEAN_AND_MEAN to exclude rarely
// used headers, and NOMINMAX to suppress the intrusive min() and max() macros.
// These macros are problematic because they interfere with standard library
// functions and templates. Note that we restore the macro state afterward to
// avoid affecting other translation units.
//
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  ifndef NOMINMAX
#    define NOMINMAX
#    include <windows.h>
#    undef NOMINMAX
#  else
#    include <windows.h>
#  endif
#  undef WIN32_LEAN_AND_MEAN
#else
#  ifndef NOMINMAX
#    define NOMINMAX
#    include <windows.h>
#    undef NOMINMAX
#  else
#    include <windows.h>
#  endif
#endif

namespace iw4x
{
  namespace utility
  {
    // Attaches the calling process to the console of the specified process
    // as a client application.
    //
    // chromium/src/base/process/launch_win.cc
    //
    void
    attach_console ();
  }
}
