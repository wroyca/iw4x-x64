#pragma once

#if defined(LIBIW4X_DISCORD_STATIC)
#  define LIBIW4X_DISCORD_SYMEXPORT
#elif defined(LIBIW4X_DISCORD_STATIC_BUILD)
#  define LIBIW4X_DISCORD_SYMEXPORT
#elif defined(LIBIW4X_DISCORD_SHARED)
#  ifdef _WIN32
#    define LIBIW4X_DISCORD_SYMEXPORT __declspec (dllimport)
#  else
#    define LIBIW4X_DISCORD_SYMEXPORT
#  endif
#elif defined(LIBIW4X_DISCORD_SHARED_BUILD)
#  ifdef _WIN32
#    define LIBIW4X_DISCORD_SYMEXPORT __declspec (dllexport)
#  else
#    define LIBIW4X_DISCORD_SYMEXPORT
#  endif
#else
// If none of the above macros are defined, then we assume we are being used
// by some third-party build system that cannot/doesn't signal the library
// type being linke.
//
#  error define LIBIW4X_DISCORD_STATIC or LIBIW4X_DISCORD_SHARED preprocessor macro to signal libiw4x-discord library type being linked
#endif
