#pragma once

#include <libiw4x/utility/utility-win32.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  extern "C"
  {
    BOOL WINAPI
    DllMain (HINSTANCE instance, DWORD reason, LPVOID reserved);
  }
}
