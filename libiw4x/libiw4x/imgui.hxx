#pragma once

#include <libiw4x/renderer.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  class LIBIW4X_SYMEXPORT imgui
  {
  public:
    explicit
    imgui (renderer& r);

  private:
    // Frame rendering callback
    //
    void
    render_frame (IDirect3DDevice9* device);

    // Window procedure hook for ImGui message handling
    //
    static LRESULT CALLBACK
    wnd_proc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
  };
}
