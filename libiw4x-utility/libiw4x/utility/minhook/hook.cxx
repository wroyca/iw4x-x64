#include <libiw4x/utility/minhook/hook.hxx>

#include <mutex>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    namespace minhook
    {
      namespace
      {
        inline void
        check_status (MH_STATUS s)
        {
          if (s != MH_OK)
            throw hook_error (static_cast<status> (s));
        }

        once_flag mh_initialize;
        once_flag mh_uninitialize;
      }

      const char*
      to_string (status s) noexcept
      {
        return MH_StatusToString (static_cast<MH_STATUS> (s));
      }

      // hook_error
      //

      hook_error::hook_error (status s)
        : runtime_error (to_string (s)), code (s)
      {
      }

      hook_error::hook_error (status s, const std::string& d)
        : runtime_error (d), code (s)
      {
      }

      hook_error::hook_error (status s, const char* d)
        : runtime_error (d), code (s)
      {
      }

      // MinHook functions
      //

      void
      initialize ()
      {
        call_once (mh_initialize,
                   []
        {
          check_status (MH_Initialize ());
        });
      }

      void
      uninitialize () noexcept
      {
        call_once (mh_uninitialize,
                   []
        {
          check_status (MH_Uninitialize ());
        });
      }

      void
      create (void*& target, void* detour, activation mode)
      {
        LPVOID o (nullptr);

        check_status (MH_CreateHook (target, detour, &o));

        if (mode == activation::queued)
          check_status (MH_QueueEnableHook (target));
        else
          check_status (MH_EnableHook (target));

        target = o;
      }

      void
      enable (void* target)
      {
        check_status (MH_EnableHook (target));
      }

      void
      disable (void* target)
      {
        check_status (MH_DisableHook (target));
      }

      void
      remove (void* target)
      {
        check_status (MH_RemoveHook (target));
      }
    }
  }
}
