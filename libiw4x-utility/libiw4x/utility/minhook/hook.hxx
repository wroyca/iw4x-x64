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
      // Concept for function pointer types.
      //
      template <typename T>
      concept fp = std::is_function_v<std::remove_pointer_t<T>>;

      // MinHook status codes wrapped as an enum class.
      //
      enum class status: int
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
      to_string (status) noexcept;

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

      // Hook activation mode.
      //
      enum class activation
      {
        immediate, queued
      };

      LIBIW4X_UTILITY_SYMEXPORT void
      initialize ();

      LIBIW4X_UTILITY_SYMEXPORT void
      uninitialize () noexcept;

      LIBIW4X_UTILITY_SYMEXPORT void*
      create (void* target,
              void* detour,
              activation mode = activation::immediate);

      LIBIW4X_UTILITY_SYMEXPORT void*
      create (uintptr_t target,
              uintptr_t detour,
              activation mode = activation::immediate);

      inline void*
      create (fp auto target,
              fp auto detour,
              activation mode = activation::immediate)
      {
        void* t = reinterpret_cast<void*> (target);
        void* d = reinterpret_cast<void*> (detour);

        return create (t, d, mode);
      }

      LIBIW4X_UTILITY_SYMEXPORT void*
      create (const wchar_t* module,
              const char* function,
              void* detour,
              activation mode = activation::immediate);

      LIBIW4X_UTILITY_SYMEXPORT void*
      create (const wchar_t* module,
              const char* function,
              uintptr_t detour,
              activation mode = activation::immediate);

      inline void*
      create (const wchar_t* module,
              const char* function,
              fp auto detour,
              activation mode = activation::immediate)
      {
        void* d = reinterpret_cast<void*> (detour);

        return create (module, function, d, mode);
      }

      LIBIW4X_UTILITY_SYMEXPORT void*
      create (const wchar_t* module,
              const char* function,
              void* detour,
              void*& target,
              activation mode = activation::immediate);

      LIBIW4X_UTILITY_SYMEXPORT void*
      create (const wchar_t* module,
              const char* function,
              uintptr_t detour,
              void*& target,
              activation mode = activation::immediate);

      inline void*
      create (const wchar_t* module,
              const char* function,
              fp auto detour,
              void*& target,
              activation mode = activation::immediate)
      {
        void* d = reinterpret_cast<void*> (d);

        return create (module, function, d, target, mode);
      }

      LIBIW4X_UTILITY_SYMEXPORT void
      disable (void* target);

      // Disable and remove the hook.
      //
      // This operation is intentionally provided for exceptional situations
      // only. The normal runtime model for IW4x is to keep hooks installed for
      // the lifetime of the process. Trying to remove hooks at arbitrary times
      // can therefore introduce hard-to-diagnose failures.
      //
      // Prefer to fix the underlying logic so that the hook does not need to be
      // torn down dynamically. Call remove() only when the problem being solved
      // is explicitly "we must detach this hook now".
      //
      // Throws hook_error on failure.
      //
      LIBIW4X_UTILITY_SYMEXPORT void
      remove (void* target);
    }
  }
}
