#pragma once

#if defined(LIBIW4X_STATIC)
#  define LIBIW4X_SYMEXPORT
#elif defined(LIBIW4X_STATIC_BUILD)
#  define LIBIW4X_SYMEXPORT
#elif defined(LIBIW4X_SHARED)
#  ifdef _WIN32
#    define LIBIW4X_SYMEXPORT __declspec (dllimport)
#  else
#    define LIBIW4X_SYMEXPORT
#  endif
#elif defined(LIBIW4X_SHARED_BUILD)
#  ifdef _WIN32
#    define LIBIW4X_SYMEXPORT __declspec (dllexport)
#  else
#    define LIBIW4X_SYMEXPORT
#  endif
#else
// If none of the above macros are defined, then we assume we are being used
// by some third-party build system that cannot/doesn't signal the library
<<<<<<< HEAD
// type being linked.
||||||| parent of ff1dd98 (Generate header containing IAT slot addresses as constants)
// type.
=======
// type.
>>>>>>> ff1dd98 (Generate header containing IAT slot addresses as constants)
//
#  error define LIBIW4X_STATIC or LIBIW4X_SHARED preprocessor macro to signal libiw4x library type being linked
#endif
