#include <libiw4x/utility/utility-win32.hxx>

#include <iostream>
#include <memory>

extern "C"
{
  #include <io.h>
}

using namespace std;

namespace iw4x
{
  namespace utility
  {
    void
    attach_console ()
    {
      // The subtlety here is that Windows has many ways to end up with stdout
      // and stderr pointing *somewhere* (sometimes to an actual console,
      // sometimes to a pipe, sometimes to a completely invalid handle). We do
      // not want to attach to `CONOUT$` in cases where the existing handles are
      // already valid and intentional, as this would silently discard the real
      // output sink.
      //
      // Instead, we first check `_fileno(stdout)` and `_fileno(stderr)`. The
      // MSVCRT sets these up at startup and will return `-2`
      // (`_NO_CONSOLE_FILENO`) if the file is invalid. This check is more
      // trustworthy than calling `GetStdHandle()`, which can return stale
      // handle IDs that may already be reused for unrelated objects by the time
      // we run.
      //
      if (_fileno (stdout) >= 0 || _fileno (stderr) >= 0)
      {
        // If either `_fileno()` is valid, we go one step further: `_fileno()`
        // itself had a bug (http://crbug.com/358267) in SUBSYSTEM:WINDOWS
        // builds for certain MSVC versions (VS2010 to VS2013), so we
        // double-check by calling `_get_osfhandle()` to confirm that the
        // underlying OS handle is valid. Only if both streams are invalid do we
        // attempt to attach a console.
        //
        intptr_t stdout_handle (_get_osfhandle (_fileno (stdout)));
        intptr_t stderr_handle (_get_osfhandle (_fileno (stderr)));

        if (stdout_handle >= 0 || stderr_handle >= 0)
          return; // console is already attached
      }

      // At this point, we've confirmed that neither standard stream is in use,
      // so we can now safely attach to the parent process's console.
      //
      if (AttachConsole (ATTACH_PARENT_PROCESS) == 0)
      {
        unsigned int e (GetLastError ());

        // `ERROR_ACCESS_DENIED` here usually means "you are already attached".
        // In that case, doing anything else would be redundant at best and
        // risky at worst, so we bail out.
        //
        if (e == ERROR_ACCESS_DENIED)
          return;

        // If we get ERROR_GEN_FAILURE, then the parent process's console is
        // effectively unusable, often because the parent crashed or otherwise
        // vanished. Creating a new console in this case would only produce an
        // orphan window with no practical audience.
        //
        if (e == ERROR_GEN_FAILURE)
          return;
      }

      // Once attached, we rebind `stdout` and `stderr` to `CONOUT$` using
      // `freopen()`. We also `_dup2()` the file descriptors (1 for stdout, 2
      // for stderr) so that any code using the raw FD API sees the same
      // handles.
      //
      // Note: failed that, there is not much we can do. We avoid throwing
      // exceptions, as this does not impact iw4x's core functionality and
      // diagnostic output is not possible, since rebind is unavailable.
      // This is a best-effort redirection; all errors are suppressed
      // unconditionally.
      //
      bool stdout_rebound (false);
      bool stderr_rebound (false);

      if (freopen ("CONOUT$", "w", stdout) != nullptr &&
          _dup2 (_fileno (stdout), 1) != -1)
        stdout_rebound = true;

      if (freopen ("CONOUT$", "w", stderr) != nullptr &&
          _dup2 (_fileno (stderr), 2) != -1)
        stderr_rebound = true;

      // If stream were rebound, realign iostream objects (`cout`, `cerr`, etc.)
      // with the C FILE streams. Failed that, both could buffer independently
      // and produce confusingly interleaved output.
      //
      if (stdout_rebound && stderr_rebound)
        ios::sync_with_stdio ();
    }

    string
    format_message (unsigned long code)
    {
      struct deleter
      {
        void
        operator() (char* p) const
        {
          LocalFree (p);
        }
      };

      // We rely on FormatMessage() to render human-readable diagnostics from
      // Windows error codes. While the API is somewhat archaic, it is still
      // the canonical Windows mechanism. That is, it allocates the buffer for
      // us (via LocalAlloc) when FORMAT_MESSAGE_ALLOCATE_BUFFER is specified.
      //
      // This allocation model is awkward: it forces us to pair with LocalFree
      // rather than standard C++ delete[], and so we wrap it in a custom
      // deleter (above) to manage ownership via unique_ptr.
      //
      // Note that If FormatMessage() itself fails (e.g., unknown error code
      // or no buffer could be allocated), then we fall back to a synthesized
      // string that at least preserves the numeric error code for
      // diagnostics.
      //
      char* message (nullptr);
      if (!FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_FROM_SYSTEM |
                          FORMAT_MESSAGE_IGNORE_INSERTS |
                          FORMAT_MESSAGE_MAX_WIDTH_MASK,
                          nullptr,
                          code,
                          MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
                          reinterpret_cast<char*> (&message),
                          0,
                          nullptr))
      {
        return "unknown error code " + to_string (code);
      }

      unique_ptr<char, deleter> _ (message);
      return message;
    }
  }
}
