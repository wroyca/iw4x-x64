#pragma once

#include <ios>

#include <io.h>

// Include Windows.h in a way that avoids the usual namespace pollution.
//
// In particular, we temporarily define WIN32_LEAN_AND_MEAN to exclude rarely
// used headers, and NOMINMAX to suppress the intrusive min() and max() macros.
// These macros are problematic because they interfere with standard library
// functions and templates. Note that we restore the macro state afterward to
// avoid affecting other translation units.
//
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  ifndef NOMINMAX
#    define NOMINMAX
#    include <windows.h>
#    undef NOMINMAX
#  else
#    include <windows.h>
#  endif
#  undef WIN32_LEAN_AND_MEAN
#else
#  ifndef NOMINMAX
#    define NOMINMAX
#    include <windows.h>
#    undef NOMINMAX
#  else
#    include <windows.h>
#  endif
#endif

namespace iw4x
{
  // Attaches the calling process to the console of the specified process
  // as a client application.
  //
  // chromium/src/base/process/launch_win.cc
  //
  inline void
  attach_console ()
  {
    // The subtlety here is that Windows has many ways to end up with
    // stdout/stderr pointing *somewhere* (sometimes to an actual console,
    // sometimes to a pipe, sometimes to a completely invalid handle). We do
    // not want to blithely attach to `CONOUT$` in cases where the existing
    // handles are already valid and intentional, as this would silently
    // discard the real output sink.
    //
    // Instead, we first check `_fileno(stdout)` and `_fileno(stderr)`. The
    // MSVCRT sets these up at startup and will return `-2`
    // (`_NO_CONSOLE_FILENO`) if the file is invalid. This check is more
    // trustworthy than calling `GetStdHandle()`, which can return stale handle
    // IDs that may already be reused for unrelated objects by the time we run.
    //
    if (_fileno (stdout) >= 0 || _fileno (stderr) >= 0)
    {
      // If either `_fileno()` is valid, we go one step further: `_fileno()`
      // itself had a bug (http://crbug.com/358267) in SUBSYSTEM:WINDOWS builds
      // for certain MSVC versions (VS2010 to VS2013), so we double-check by
      // calling `_get_osfhandle()` to confirm that the underlying OS handle is
      // valid. Only if both streams are invalid do we attempt to attach a
      // console.
      //
      intptr_t stdout_handle (_get_osfhandle (_fileno (stdout)));
      intptr_t stderr_handle (_get_osfhandle (_fileno (stderr)));

      if (stdout_handle >= 0 || stderr_handle >= 0)
        return;
    }

    // At this point, we've confirmed that neither standard stream is in use,
    // so we can now safely attach to the parent process's console.
    //
    if (AttachConsole (ATTACH_PARENT_PROCESS) == 0)
    {
      unsigned int result (GetLastError ());

      // `ERROR_ACCESS_DENIED` here usually means "you are already attached".
      // In that case, doing anything else would be redundant at best and risky
      // at worst, so we bail out.
      //
      if (result == ERROR_ACCESS_DENIED)
        return;

      // If we get ERROR_GEN_FAILURE, then the parent process's console is
      // effectively unusable, often because the parent crashed or otherwise
      // vanished. Creating a new console in this case would only produce an
      // orphan window with no practical audience.
      //
      if (result == ERROR_GEN_FAILURE)
        return;
    }

    // Once attached, we rebind `stdout` and `stderr` to `CONOUT$` using
    // `freopen()` and set a reasonably large line buffer (64 KiB). The large
    // buffer minimizes interleaving of log lines from different threads. We
    // also `_dup2()` the file descriptors (1 for stdout, 2 for stderr) so that
    // any code using the raw FD API sees the same handles.
    //
    {
      enum
      {
        output_buffer_size = 64 * 1024
      };

      if (freopen ("CONOUT$", "w", stdout) != nullptr)
      {
        (void) setvbuf (stdout, nullptr, _IOLBF, output_buffer_size);
        _dup2 (_fileno (stdout), 1);
      }

      if (freopen ("CONOUT$", "w", stderr) != nullptr)
      {
        (void) setvbuf (stderr, nullptr, _IOLBF, output_buffer_size);
        _dup2 (_fileno (stderr), 2);
      }
    }

    // Finally, we must realign iostream objects (`cout`, `cerr`, etc.) with
    // the C FILE streams. Failed that, both could buffer independently and
    // produce confusingly interleaved output.
    //
    std::ios::sync_with_stdio ();
  }
}
