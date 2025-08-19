#pragma once

#include <libiw4x/utility/windows.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
  }
}
