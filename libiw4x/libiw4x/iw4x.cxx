#include <libiw4x/iw4x.hxx>

namespace iw4x
{
  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    {
      if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

      // Successful DLL_PROCESS_ATTACH.
      //
      return TRUE;
    }
  }
}
