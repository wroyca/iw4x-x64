#include <libiw4x/d3d9/d3d9.hxx>

using namespace std;

namespace iw4x
{
  namespace d3d9
  {
    extern "C"
    {
      IDirect3D9* WINAPI
      create (unsigned int sdk_version)
      {
        // ...
      }

      int WINAPI
      begin_event (unsigned long color, const wchar_t* name)
      {
        // ...
      }

      int WINAPI
      end_event ()
      {
        // ...
      }
    }
  }
}
