#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

#include <libiw4x/utility/minhook/minhook.h>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  namespace utility
  {
    namespace minhook
    {
      // MinHook status codes wrapped as an enum class.
      //
      enum class status : int
      {
        ok                       = MH_OK,
        already_initialized      = MH_ERROR_ALREADY_INITIALIZED,
        not_initialized          = MH_ERROR_NOT_INITIALIZED,
        already_created          = MH_ERROR_ALREADY_CREATED,
        not_created              = MH_ERROR_NOT_CREATED,
        enabled                  = MH_ERROR_ENABLED,
        disabled                 = MH_ERROR_DISABLED,
        not_executable           = MH_ERROR_NOT_EXECUTABLE,
        unsupported_function     = MH_ERROR_UNSUPPORTED_FUNCTION,
        memory_alloc             = MH_ERROR_MEMORY_ALLOC,
        memory_protect           = MH_ERROR_MEMORY_PROTECT,
        module_not_found         = MH_ERROR_MODULE_NOT_FOUND,
        function_not_found       = MH_ERROR_FUNCTION_NOT_FOUND,
        unknown                  = MH_UNKNOWN
      };

      // Convert status to string representation.
      //
      LIBIW4X_UTILITY_SYMEXPORT const char*
      to_string (status);

      // Exception thrown on MinHook errors.
      //
      class LIBIW4X_UTILITY_SYMEXPORT hook_error: public std::runtime_error
      {
      public:
        status code;

        explicit
        hook_error (status);

        explicit
        hook_error (status, const std::string& description);

        explicit
        hook_error (status, const char* description);
      };

      LIBIW4X_UTILITY_SYMEXPORT void
      initialize ();

      LIBIW4X_UTILITY_SYMEXPORT void
      uninitialize ();

      LIBIW4X_UTILITY_SYMEXPORT void
      create (uintptr_t target, uintptr_t detour);

      LIBIW4X_UTILITY_SYMEXPORT void
      create (void* target, void* detour);

      LIBIW4X_UTILITY_SYMEXPORT void
      create (void*& target, void* detour);

      template <typename T, typename S>
        requires (std::is_pointer_v<T> && std::is_pointer_v<S> &&
                  std::is_function_v<std::remove_pointer_t<T>> &&
                  std::is_function_v<std::remove_pointer_t<S>>)
      LIBIW4X_UTILITY_SYMEXPORT inline void
      create (T& t, S s)
      {
        create (reinterpret_cast<void*> (t), reinterpret_cast<void*> (s));

        // Force selection of create(void*, void*). Passing both arguments as
        // prvalues avoids the ambiguity between the void* and void*& overloads
        // while still allowing MinHook to install the patch.
        //
        // Because the target is passed as a prvalue, MinHook cannot propagate
        // the trampoline address back into the caller's function-pointer
        // variable. We therefore reassign it explicitly after the call to
        // preserve the usual "original function" semantics.
        //
        t = reinterpret_cast<T> (t);
      }
    }
  }
}
