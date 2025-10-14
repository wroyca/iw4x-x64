#include <libiw4x/iw4x.hxx>

namespace iw4x
{
  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE instance, DWORD reason, LPVOID reserved)
    {
      if (reason != DLL_PROCESS_ATTACH)
        return TRUE;

      // Successful DLL_PROCESS_ATTACH.
      //
      return TRUE;
    }
  }
}
