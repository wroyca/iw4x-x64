#pragma once

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
