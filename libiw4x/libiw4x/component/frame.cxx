#include <libiw4x/component/frame.hxx>

#include <libiw4x/utility/minhook/hook.hxx>

#include <libiw4x/symbol.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
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

    // Note that we lie here. The engine's nominal frame boundary is Com_Frame,
    // but the actual control path is mediated by Com_Frame_Try_Block_Function.
    // We instrument the latter.
    //
    s.register_pipeline ("com_frame");

    // Hook Com_Frame_Try_Block_Function.
    //
    minhook::create (Com_Frame_Try_Block_Function,
                     &com_frame_try_block_function_stub);
  }
}
