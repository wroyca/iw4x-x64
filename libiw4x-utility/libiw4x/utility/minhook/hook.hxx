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
      create (void*& target, void* source);

      template <typename Target, typename Source>
        requires (std::is_function_v<std::remove_pointer_t<Target>> &&
                  std::is_function_v<std::remove_pointer_t<Source>>)
      LIBIW4X_UTILITY_SYMEXPORT inline void
      create (Target& target, Source source)
      {
        void* t = reinterpret_cast<void*> (target);
        void* s = reinterpret_cast<void*> (source);

        create (t, s);

        target = reinterpret_cast<decltype (target)> (t);
      }
    }
  }
}
