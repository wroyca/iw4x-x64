#pragma once

#include <d3d9.h>

#include <libiw4x/utility/utility-win32.hxx>

#include <libiw4x/d3d9/export.hxx>

namespace iw4x
{
  namespace d3d9
  {
    extern "C"
    {
      IDirect3D9* WINAPI
      create (unsigned int sdk_version);

      int WINAPI
      begin_event (unsigned long color, const wchar_t* name);

      int WINAPI
      end_event ();
    }
  }
}
