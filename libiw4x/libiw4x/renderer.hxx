#pragma once

#include <boost/signals2.hpp>

#include <libiw4x/export.hxx>

// Forward declarations for D3D9.
//
struct IDirect3DDevice9;
struct IDirect3D9;

namespace iw4x
{
  class LIBIW4X_SYMEXPORT renderer
  {
  public:
    using signal = boost::signals2::signal<void (IDirect3DDevice9*)>;

    renderer ();

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
    signal frame_signal_;

    // Dispatch frame notifications to all connected slots.
    //
    void
    notify (IDirect3DDevice9*);

    friend void
    dispatch_frame (IDirect3DDevice9*);
  };

  // Global dispatcher for frame events.
  //
  void
  dispatch_frame (IDirect3DDevice9*);
}
