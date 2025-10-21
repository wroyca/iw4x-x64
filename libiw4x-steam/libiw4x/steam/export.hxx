#pragma once

#if defined(LIBIW4X_STEAM_STATIC)
#  define LIBIW4X_STEAM_SYMEXPORT
#elif defined(LIBIW4X_STEAM_STATIC_BUILD)
#  define LIBIW4X_STEAM_SYMEXPORT
#elif defined(LIBIW4X_STEAM_SHARED)
#  ifdef _WIN32
#    define LIBIW4X_STEAM_SYMEXPORT __declspec(dllimport)
#  else
#    define LIBIW4X_STEAM_SYMEXPORT
#  endif
#elif defined(LIBIW4X_STEAM_SHARED_BUILD)
#  ifdef _WIN32
#    define LIBIW4X_STEAM_SYMEXPORT __declspec(dllexport)
#  else
#    define LIBIW4X_STEAM_SYMEXPORT
#  endif
#else
// If none of the above macros are defined, then we assume we are being used
// by some third-party build system that cannot/doesn't signal the library
// type being linked.
//
#  error define LIBIW4X_STEAM_STATIC or LIBIW4X_STEAM_SHARED preprocessor macro to signal libiw4x-steam library type being linked
#endif
