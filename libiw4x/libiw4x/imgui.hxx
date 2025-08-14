#pragma once

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <libiw4x/export.hxx>
#include <libiw4x/utility-win32.hxx>

namespace iw4x
{
  class LIBIW4X_SYMEXPORT imgui
  {
  public:
    explicit
    imgui ();

  private:
    void
    intercept_d3d9 ();

    static IDirect3D9* WINAPI
    d3d9_create (unsigned int sdk_version);

    // Static instance for callbacks
    //
    static imgui* instance_;
  };
}
