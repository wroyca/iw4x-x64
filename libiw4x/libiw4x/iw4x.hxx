#pragma once

#include <iosfwd>
#include <string>

#include <libiw4x/export.hxx>

#include <libiw4x/utility-win32.hxx>

namespace iw4x
{
  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
  }
}
