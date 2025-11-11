#pragma once

#include <cstddef>
#include <cstdint>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  namespace utility
  {
    // Set memory to a specified value, unprotecting it first if necessary.
    //
    LIBIW4X_UTILITY_SYMEXPORT void*
    memset (void* destination, int value, size_t);

    // Convenience overload accepting address as uintptr_t.
    //
    LIBIW4X_UTILITY_SYMEXPORT void*
    memset (uintptr_t destination, int value, size_t);
  }
}
