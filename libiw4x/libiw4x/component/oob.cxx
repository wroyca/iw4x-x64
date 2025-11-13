#include <libiw4x/component/oob.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  namespace
  {
    scheduler* sched (nullptr);
  }

  oob::
  oob (scheduler& s)
  {
    sched = &s;
  }
}
