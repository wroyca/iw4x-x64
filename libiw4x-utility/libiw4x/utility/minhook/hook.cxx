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
            [[unlikely]] throw hook_error (static_cast<status> (s));
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

      hook_error::hook_error (status s, const string& d)
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
      uninitialize ()
      {
        call_once (mh_uninitialize,
                   []
        {
          check_status (MH_Uninitialize ());
        });
      }

      void
      create (void*& target, void* source)
      {
        void* o (nullptr);

        check_status (MH_CreateHook (target, source, &o));
        check_status (MH_EnableHook (target));

        target = o;
      }
    }
  }
}
