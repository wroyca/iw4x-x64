#include <libiw4x/iw4x.hxx>

using namespace std;

namespace iw4x
{
  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    {
      if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

      uintptr_t source (0x140358EBC);
      uintptr_t target (reinterpret_cast<uintptr_t> (+[] ()
      {
        // __security_init_cookie
        //
        reinterpret_cast<void (*) ()> (0x1403598CC) ();

        // __scrt_common_main_seh
        //
        return reinterpret_cast<int (*) ()> (0x140358D48) ();
      }));

      // Successful DLL_PROCESS_ATTACH.
      //
      return TRUE;
    }
  }
}
