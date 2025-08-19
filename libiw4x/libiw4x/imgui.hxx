#pragma once

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <libiw4x/utility/windows.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  class LIBIW4X_SYMEXPORT imgui
  {
  public:
    explicit imgui ();

    // Check if ImGui is initialized and ready for rendering.
    //
    bool
    is_initialized () const;

    // Check if device objects are created and valid.
    //
    bool
    has_device_objects () const;

  private:
    void
    initialize (HWND window, IDirect3DDevice9 *device);

    bool
    process_message (HWND window,
                     unsigned int message,
                     unsigned long long wparam,
                     long long lparam) const;

    void
    render ();

    void
    reset_device ();

    void
    create_device_objects ();

    // D3D9 function type definitions to store original vtable/function
    // pointers.
    //
    using reset_function         = HRESULT (WINAPI *) (IDirect3DDevice9 *,
                                                       D3DPRESENT_PARAMETERS *);
    using endscene_function      = HRESULT (WINAPI *) (IDirect3DDevice9 *);
    using create_device_function = HRESULT (WINAPI *) (IDirect3D9 *,
                                                       unsigned int,
                                                       D3DDEVTYPE,
                                                       HWND,
                                                       unsigned long,
                                                       D3DPRESENT_PARAMETERS *,
                                                       IDirect3DDevice9 **);

    void
    intercept_d3d9 ();

    void **
    get_vtable (void *object);

    static void
    intercept_vtable (void *address,
                      void *new_value,
                      size_t size = sizeof (void *));

    static IDirect3D9 *WINAPI
    d3d9_create (unsigned int sdk_version);

    static HRESULT WINAPI
    d3d9_create_device (IDirect3D9 *d3d9,
                        unsigned int adapter,
                        D3DDEVTYPE device_type,
                        HWND focus_window,
                        unsigned long behavior_flags,
                        D3DPRESENT_PARAMETERS *presentation_parameters,
                        IDirect3DDevice9 **device);

    static HRESULT WINAPI
    d3d9_reset (IDirect3DDevice9 *device,
                D3DPRESENT_PARAMETERS *presentation_parameters);

    static HRESULT WINAPI
    d3d9_endscene (IDirect3DDevice9 *device);

    static long long CALLBACK
    window_procedure (HWND window,
                      unsigned int message,
                      unsigned long long wparam,
                      long long lparam);

    void
    intercept_window (HWND window);

    // Initialization and device state
    //
    bool initialized_;
    bool device_objects_created_;
    bool demo_window_visible_;

    // References to the host window and device. Not owned.
    //
    HWND window_handle_;
    IDirect3DDevice9 *d3d9_device_;

    // Original function pointers saved so we can forward to them from our
    // interceptors.
    //
    reset_function original_reset_;
    endscene_function original_endscene_;
    create_device_function original_create_device_;

    // Window procedure state
    //
    WNDPROC original_window_procedure_;
    HWND hooked_window_;

    // Static instance for callbacks.
    //
    static imgui *instance_;
  };
}
