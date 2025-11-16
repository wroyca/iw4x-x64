#include <libiw4x/mod/frame.hxx>

#include <libiw4x/utility/minhook/hook.hxx>

#include <libiw4x/sym/com.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  namespace mod
  {
    namespace
    {
      scheduler* sched (nullptr);

      void
      com_frame_try_block_function_stub ()
      {
        struct poll
        {
          ~poll ()
          {
            if (sched)
            {
              // Note that we lie here. The engine's nominal frame boundary is
              // Com_Frame, but the actual control path is mediated by
              // Com_Frame_Try_Block_Function. We instrument the latter.
              //
              sched->poll ("com_frame");
            }
          }
        };

        // Scheduler poll must run after the frame boundary.
        //
        poll p;

        // Call original Com_Frame_Try_Block_Function.
        //
        Com_Frame_Try_Block_Function ();
      }
    }

    frame::
    frame (scheduler& s)
    {
      sched = &s;

      s.register_pipeline ("com_frame");

      minhook::create (Com_Frame_Try_Block_Function, &com_frame_try_block_function_stub);
    }
  }
}
