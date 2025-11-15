#pragma once

#include <libiw4x/utility/utility-win32.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  namespace windows
  {
    // Install detours for the Windows ProcessThreadsAPI.
    //
    void
    process_threads_api_init ();
  }
}
