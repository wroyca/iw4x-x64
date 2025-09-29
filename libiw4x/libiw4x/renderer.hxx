#pragma once

#include <boost/signals2.hpp>

#include <libiw4x/utility/utility-win32.hxx>

#include <libiw4x/export.hxx>

// Forward declarations for D3D9
//
struct IDirect3DDevice9;
struct IDirect3D9;

namespace iw4x
{
  class LIBIW4X_SYMEXPORT renderer
  {
  public:
    renderer ();

    // Signal type for frame events
    //
    using signal = boost::signals2::signal<void (IDirect3DDevice9*)>;

    // Get the frame rendering signal for connecting callbacks
    //
    // Returns a reference to the internal boost::signals2::signal that
    // fires during each frame render. We can connect our callback
    // functions to this signal to receive rendering events.
    //
    signal&
    on_frame () noexcept;

  private:
    // Internal signal for frame events
    //
    signal frame_signal;

    // Friend function for DirectX hook
    //
    friend void notify_frame_render (IDirect3DDevice9* device);
  };

  // Global function to dispatch frame rendering events
  //
  void
  notify_frame_render (IDirect3DDevice9* device);
}
