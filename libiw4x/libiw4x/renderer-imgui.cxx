#include <libiw4x/renderer-imgui.hxx>

#include <d3d9.h>
#ifdef _MSC_VER
#  pragma comment(lib, "d3d9.lib")
#endif

#include <imgui.h>
#include <imgui/backends/imgui_impl_dx9.h>
#include <imgui/backends/imgui_impl_win32.h>

#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>

// Forward declare the ImGui Win32 message handler. See imgui_impl_win32.h for
// context.
//
extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler (HWND, UINT, WPARAM, LPARAM);

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  namespace
  {
    once_flag imgui_init;

    WNDPROC original_wnd_proc_ (nullptr);
  }

  imgui::imgui (renderer& r)
  {
    r.on_frame ().connect ([this] (IDirect3DDevice9* d)
    {
      // Initialize ImGui lazily on the first frame. We avoid frontloading this
      // work during renderer construction since we need a valid device anyway,
      // and there is no point in doing it twice if the first frame never
      // arrives.
      //
      call_once (imgui_init, [d] ()
      {
        HRESULT hr;

        // To integrate ImGui we need the window handle that owns the device.
        // Direct3D does not hand this out directly; the swap chain is the
        // sanctioned path. The detour feels roundabout but avoids maintaining
        // parallel knowledge of windowing state.
        //
        IDirect3DSwapChain9* sc;
        hr = d->GetSwapChain (0, &sc);

        if (FAILED (hr))
          throw runtime_error ("unable to retrieve swap chain");

        D3DPRESENT_PARAMETERS pp;
        hr = sc->GetPresentParameters (&pp);
        sc->Release ();

        if (FAILED (hr))
          throw runtime_error ("unable to retrieve presentation parameters");

        HWND hwnd (pp.hDeviceWindow);

        // With the handle resolved we can commit to an ImGui context. Input and
        // docking are enabled up front to avoid layering configuration later.
        // Note that we rely on ImGui's defaults for most behavior.
        //
        ImGui::CreateContext ();

        if (!ImGui_ImplWin32_Init (hwnd))
          throw runtime_error ("unable to initialize ImGui Win32 backend");

        if (!ImGui_ImplDX9_Init (d))
          throw runtime_error ("unable to initialize ImGui DirectX9 backend");

        ImGuiIO& io (ImGui::GetIO ());
        io.MouseDrawCursor = true;

        // Hook window procedure to forward messages to ImGui.
        //
        original_wnd_proc_ = reinterpret_cast<WNDPROC> (
          SetWindowLongPtrW (hwnd,
                             GWLP_WNDPROC,
                             reinterpret_cast<LONG_PTR> (wnd_proc)));

        if (original_wnd_proc_ == nullptr)
        {
          ostringstream s;
          unsigned long e (GetLastError ());

          s << "unable to hook window procedure: "
            << format_message (e) << endl;

          throw runtime_error (s.str ());
        }
      });

      render_frame (d);
    });
  }

  void
  imgui::render_frame (IDirect3DDevice9* device)
  {
    // Start the ImGui frame
    //
    ImGui_ImplDX9_NewFrame ();
    ImGui_ImplWin32_NewFrame ();
    ImGui::NewFrame ();

    {
      static bool show_demo_window (true);
      ImGui::ShowDemoWindow (&show_demo_window);
    }

    // Rendering closes the loop by handing ImGui's draw data back to DirectX.
    // At this point the integration is complete for the frame: ImGui advances
    // in lockstep with the renderer without maintaining separate lifecycles.
    //
    ImGui::Render ();
    ImGui_ImplDX9_RenderDrawData (ImGui::GetDrawData ());
  }

  LRESULT CALLBACK
  imgui::wnd_proc (HWND h, UINT msg, WPARAM wp, LPARAM lp)
  {
    // Forward the message to ImGui first.
    //
    ImGui_ImplWin32_WndProcHandler (h, msg, wp, lp);

    // Check if ImGui wants to capture this input. If so, block it from
    // reaching the game to prevent unintended interactions (e.g., clicking
    // game menus while interacting with ImGui windows).
    //
    ImGuiIO& io (ImGui::GetIO ());

    // Block mouse input if ImGui wants mouse capture.
    //
    if (io.WantCaptureMouse)
    {
      switch (msg)
      {
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_MBUTTONDBLCLK:
      case WM_MOUSEWHEEL:
      case WM_MOUSEHWHEEL:
      case WM_MOUSEMOVE:
        return 0;  // Block mouse input
      }
    }

    // Block keyboard input if ImGui wants keyboard capture.
    //
    if (io.WantCaptureKeyboard)
    {
      switch (msg)
      {
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
      case WM_CHAR:
        return 0;  // Block keyboard input
      }
    }

    // ImGui doesn't want this input, forward to original window procedure.
    //
    if (original_wnd_proc_ != nullptr)
      return CallWindowProcW (original_wnd_proc_, h, msg, wp, lp);

    // If no original procedure was stored, fall back to Windows default
    // handler.
    //
    return DefWindowProcW (h, msg, wp, lp);
  }
}
