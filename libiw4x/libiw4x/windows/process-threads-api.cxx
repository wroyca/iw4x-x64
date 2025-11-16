#include <libiw4x/windows/process-threads-api.hxx>

#include <iostream>

#include <libiw4x/utility/minhook/hook.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  namespace windows
  {
    namespace
    {
#ifdef __MINGW32__
      void (WINAPI* ExitProcessHk) (UINT) = nullptr;
#endif

#ifdef __MINGW32__
      // MinGW does not guarantee orderly shutdown for C++ static objects that
      // live in a DLL. The problem originates in how destructors are
      // registered. On Windows, DLLs do not participate in the atexit() chain.
      // Only the main executable does. This is intentional: DLLs can be
      // unloaded at arbitrary times, so allowing them to register atexit()
      // destructors would leave the runtime with stale function pointers.
      // Invoking them during final process teardown would dereference code that
      // no longer exists.
      //
      // The direct consequence here is that static-duration objects inside a
      // DLL never receive guaranteed destruction. Under MSVC this is usually
      // benign because thread finalization and CRT teardown follow a more
      // predictable path. Under MinGW the situation is markedly worse: the
      // runtime attempts a full unwind of thread-local and static resources
      // during ExitProcess(), but the DLL boundary confuses its finalizer and
      // stall in the shutdown path, causing the process to hang indefinitely.
      //
      // NOTE: Some crash-reporting libraries (Breakpad, Crashpad, Sentry)
      // install their own ExitProcess hooks to capture final telemetry. If we
      // ever adopt one of these, this override would bypass their shutdown
      // path. In practice this is not a concern: none of these libraries
      // support MinGW in any meaningful or maintained form, and the community
      // patches that once existed are long obsolete.
      //
      void WINAPI
      exit_process (UINT code)
      {
        HANDLE p (GetCurrentProcess ());
        TerminateProcess (p, code);
      }
#endif
    }

    void
    process_threads_api_init ()
    {
#ifdef __MINGW32__
      ExitProcessHk = &ExitProcess;
#endif

#ifdef __MINGW32__
      try
      {
        minhook::create (ExitProcessHk, &exit_process);
      }
      catch (const minhook::hook_error& e)
      {
        cerr << "error: minhook is unable to install ExitProcess: "
             << e.what () << endl;

        exit_process (1);
      }
#endif
    }
  }
}
