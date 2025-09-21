#include <libiw4x/iw4x.hxx>

using namespace std;

namespace iw4x
{
  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    {
      // Perform actions based on the reason for calling.
      //
      switch (fdwReason)
      {
        case DLL_PROCESS_ATTACH:
          break;

        case DLL_THREAD_ATTACH:
          break;

        case DLL_THREAD_DETACH:
          break;

        case DLL_PROCESS_DETACH:
          break;
      }

      // Successful DLL_PROCESS_ATTACH.
      //
      return TRUE;
    }
  }
}
