#include <libiw4x/utility/stacktrace.hxx>

#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

#include <boost/stacktrace.hpp>

#include <cstdlib>
#include <signal.h>

#include <libiw4x/utility/windows.hxx>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    // Crash and terminate handling is inherently a global concern: there is
    // only one set of process-wide handlers. This means we must manage
    // installation and removal carefully to avoid either clobbering other
    // libraries or leaving dangling state behind.
    //
    // We use a global mutex to serialize handler installation since handler
    // state is shared across threads. This is not about correctness of the
    // crash path (that's always racy), but about predictable setup/teardown
    // during normal execution.
    //
    namespace
    {
      mutex handler_mutex_;
      bool crash_handler_installed_ (false);
      bool terminate_handler_installed_ (false);

      // The original handlers are stored so we can chain through. Overriding
      // blindly would be hostile. Chaining is the compromise: we add
      // diagnostics, then delegate back.
      //
      void (*original_sigabrt_handler_) (int) (nullptr);
      void (*original_sigfpe_handler_)  (int) (nullptr);
      void (*original_sigill_handler_)  (int) (nullptr);
      void (*original_sigint_handler_)  (int) (nullptr);
      void (*original_sigsegv_handler_) (int) (nullptr);
      void (*original_sigterm_handler_) (int) (nullptr);

      LPTOP_LEVEL_EXCEPTION_FILTER original_exception_filter_ (nullptr);
      PVOID vectored_exception_handler_ (nullptr);

      terminate_handler original_terminate_handler_ (nullptr);

      // Signal handler that prints stack trace.
      //
      void
      crash_signal_handler (int sig)
      {
        // Print stack trace to stderr.
        //
        cerr << "\ncrash signal detected (" << sig << "):\n";
        print_stacktrace (cerr, 1, "  ");
        cerr.flush ();

        // If an original handler exists, we call it. If no handler exists, we
        // fall back to _exit with a conventional code.
        //
        void (*original_handler) (int) (nullptr);
        switch (sig)
        {
          case SIGABRT: original_handler = original_sigabrt_handler_; break;
          case SIGFPE:  original_handler = original_sigfpe_handler_;  break;
          case SIGILL:  original_handler = original_sigill_handler_;  break;
          case SIGINT:  original_handler = original_sigint_handler_;  break;
          case SIGSEGV: original_handler = original_sigsegv_handler_; break;
          case SIGTERM: original_handler = original_sigterm_handler_; break;
        }

        if (original_handler != nullptr && original_handler != SIG_DFL)
          original_handler (sig);
        else
          _exit (128 + sig);
      }

      // Structured Exception Handling (SEH)
      //
      LONG WINAPI
      crash_exception_handler (EXCEPTION_POINTERS* info)
      {
        cerr << "\nunhandled exception detected (seh):\n";

        if (info && info->ExceptionRecord)
        {
          DWORD c (info->ExceptionRecord->ExceptionCode);

          cerr << "structured exception handler triggered:\n"
               << "exception code: 0x" << hex << c << dec;

          switch (c)
          {
            case EXCEPTION_ACCESS_VIOLATION:
            {
              cerr << " (access_violation)";
              if (info->ExceptionRecord->NumberParameters >= 2)
              {
                auto s (info->ExceptionRecord->ExceptionInformation [0]);
                auto a (info->ExceptionRecord->ExceptionInformation [1]);

                cerr << " - " << (s ? "write" : "read") << " at address 0x"
                     << hex << a << dec;
              }
              break;
            }
            case EXCEPTION_STACK_OVERFLOW:
              cerr << " (stack_overflow)";
              break;
            case EXCEPTION_ILLEGAL_INSTRUCTION:
              cerr << " (illegal_instruction)";
              break;
            case EXCEPTION_IN_PAGE_ERROR:
              cerr << " (in_page_error)";
              break;
            case EXCEPTION_INVALID_HANDLE:
              cerr << " (invalid_handle)";
              break;
            default:
              break;
          }
          cerr << '\n';
        }

        print_stacktrace (cerr, 0, "  ");
        cerr.flush ();

        // Call original filter if available.
        //
        if (original_exception_filter_ != nullptr)
          return original_exception_filter_ (info);

        return EXCEPTION_CONTINUE_SEARCH;
      }

      // Vectored Exception Handling (VEH).
      //
      // VEH is even earlier than SEH: Windows will call us as soon as
      // the exception is dispatched, before any structured handlers.
      // This gives us the earliest and least interfered-with view of
      // the crash. We use it to log, but deliberately return
      // CONTINUE_SEARCH so that SEH and other handlers still run.
      //
      // The filtering logic is important: not all exceptions are
      // "crashes". Some are used internally by debuggers or Windows
      // subsystems. Logging those would produce a lot of noise.
      //
      LONG WINAPI
      vectored_crash_handler (EXCEPTION_POINTERS *info)
      {
        if (info && info->ExceptionRecord)
        {
          DWORD c (info->ExceptionRecord->ExceptionCode);

          // Only handle actual crashes, not debugging exceptions.
          //
          switch (c)
          {
            case EXCEPTION_ACCESS_VIOLATION:
            case EXCEPTION_STACK_OVERFLOW:
            case EXCEPTION_ILLEGAL_INSTRUCTION:
            case EXCEPTION_IN_PAGE_ERROR:
            case EXCEPTION_INVALID_HANDLE:
            case EXCEPTION_GUARD_PAGE:
            case EXCEPTION_INVALID_DISPOSITION:
            case EXCEPTION_NONCONTINUABLE_EXCEPTION:
              break;
            default:
              // Let other handlers deal with non-crash exceptions.
              //
              return EXCEPTION_CONTINUE_SEARCH;
          }

          cerr << "vectored exception handler triggered:\n"
               << "exception code: 0x" << hex << c << dec;

          switch (c)
          {
            case EXCEPTION_ACCESS_VIOLATION:
            {
              cerr << " (access_violation)";
              if (info->ExceptionRecord->NumberParameters >= 2)
              {
                auto s (info->ExceptionRecord->ExceptionInformation [0]);
                auto a (info->ExceptionRecord->ExceptionInformation [1]);

                cerr << " - " << (s ? "write" : "read")
                     << " at address 0x" << hex << a << dec;
              }
              break;
            }
            case EXCEPTION_STACK_OVERFLOW:
              cerr << " (stack_overflow)";
              break;
            case EXCEPTION_ILLEGAL_INSTRUCTION:
              cerr << " (illegal_instruction)";
              break;
            case EXCEPTION_IN_PAGE_ERROR:
              cerr << " (in_page_error)";
              break;
            case EXCEPTION_INVALID_HANDLE:
              cerr << " (invalid_handle)";
              break;
            case EXCEPTION_GUARD_PAGE:
              cerr << " (guard_page)";
              break;
            case EXCEPTION_INVALID_DISPOSITION:
              cerr << " (invalid_disposition)";
              break;
            case EXCEPTION_NONCONTINUABLE_EXCEPTION:
              cerr << " (noncontinuable_exception)";
              break;
            default:
              break;
          }
          cerr << '\n';

          print_stacktrace (cerr, 0, "  ");
          cerr.flush ();
        }

        // Continue searching for other handlers.
        //
        return EXCEPTION_CONTINUE_SEARCH;
      }

      // Terminate handler.
      //
      // This covers the C++ runtime side of things. If a noexcept
      // function throws, or if an exception propagates out of main(),
      // std::terminate() is called. By default this just aborts.
      //
      // Installing our own handler means we can at least record the
      // stacktrace, which is usually the only way to diagnose these
      // failures post-mortem.
      //
      void
      crash_terminate_handler ()
      {
        cerr << "\nProgram termination detected:\n";
        print_stacktrace (cerr, 1, "  ");
        cerr.flush ();

        // Call original handler if available.
        //
        if (original_terminate_handler_ != nullptr)
          original_terminate_handler_ ();
        else
          abort ();
      }
    }

    // stacktrace
    //

    stacktrace::stacktrace ()
      : trace_ (nullptr)
    {
    }

    stacktrace::~stacktrace ()
    {
      clear ();
    }

    stacktrace::stacktrace (stacktrace &&other)
      : trace_ (other.trace_)
    {
      other.trace_ = nullptr;
    }

    stacktrace &
    stacktrace::operator= (stacktrace &&other)
    {
      if (this != &other)
      {
        clear ();
        trace_ = other.trace_;
        other.trace_ = nullptr;
      }
      return *this;
    }

    void
    stacktrace::capture (size_t max_frames)
    {
      clear ();

      boost::stacktrace::stacktrace *trace;
      if (max_frames == 0)
        trace = new boost::stacktrace::stacktrace ();
      else
        trace = new boost::stacktrace::stacktrace (0, max_frames);

      trace_ = trace;
    }

    void
    stacktrace::capture (size_t skip_frames, size_t max_frames)
    {
      clear ();

      boost::stacktrace::stacktrace *trace (
        new boost::stacktrace::stacktrace (skip_frames, max_frames));
      trace_ = trace;
    }

    bool
    stacktrace::empty () const
    {
      return trace_ == nullptr ||
        static_cast<boost::stacktrace::stacktrace *> (trace_)->empty ();
    }

    size_t
    stacktrace::size () const
    {
      return trace_ == nullptr
        ? 0
        : static_cast<boost::stacktrace::stacktrace *> (trace_)->size ();
    }

    void
    stacktrace::clear ()
    {
      if (trace_ != nullptr)
      {
        delete static_cast<boost::stacktrace::stacktrace *> (trace_);
        trace_ = nullptr;
      }
    }

    string
    stacktrace::to_string () const
    {
      if (empty ())
        return "";

      ostringstream oss;
      oss << *static_cast<boost::stacktrace::stacktrace *> (trace_);
      return oss.str ();
    }

    void
    stacktrace::print (ostream &os) const
    {
      if (!empty ())
        os << *static_cast<boost::stacktrace::stacktrace *> (trace_);
    }

    void
    stacktrace::print (ostream &os, const string &prefix) const
    {
      if (empty ())
        return;

      string str (to_string ());
      istringstream iss (str);
      string line;

      while (getline (iss, line))
        os << prefix << line << '\n';
    }

    ostream &
    operator<< (ostream &os, const stacktrace &st)
    {
      st.print (os);
      return os;
    }

    // Handler installation.
    //
    // These functions are meant to be called once at startup. We don't try to
    // be clever about re-installation in the crash path. That is, by the time
    // the process is going down, consistency doesn't matter. What matters is
    // that setup and teardown are predictable in the normal path.
    //
    // Note the order: we install VEH first (earliest), then signals, then SEH
    // as a last fallback. The strategy here is to capture as much as possible
    // at the earliest stage without blocking later handlers.
    //
    void
    install_crash_handler ()
    {
      lock_guard<mutex> lock (handler_mutex_);

      if (crash_handler_installed_)
        return;

      // Install vectored exception handler first (highest priority).
      //
      vectored_exception_handler_ =
        AddVectoredExceptionHandler (1, vectored_crash_handler);

      // Install signal handlers.
      //
      original_sigabrt_handler_ = signal (SIGABRT, crash_signal_handler);
      original_sigfpe_handler_  = signal (SIGFPE, crash_signal_handler);
      original_sigill_handler_  = signal (SIGILL, crash_signal_handler);
      original_sigint_handler_  = signal (SIGINT, crash_signal_handler);
      original_sigsegv_handler_ = signal (SIGSEGV, crash_signal_handler);
      original_sigterm_handler_ = signal (SIGTERM, crash_signal_handler);

      // Install structured exception handler as fallback.
      //
      original_exception_filter_ =
        SetUnhandledExceptionFilter (crash_exception_handler);

      crash_handler_installed_ = true;
    }

    void
    install_terminate_handler ()
    {
      lock_guard<mutex> lock (handler_mutex_);

      if (terminate_handler_installed_)
        return;

      original_terminate_handler_ = set_terminate (crash_terminate_handler);
      terminate_handler_installed_ = true;
    }

    void
    remove_crash_handlers ()
    {
      lock_guard<mutex> lock (handler_mutex_);

      // Remove vectored exception handler.
      //
      if (vectored_exception_handler_ != nullptr)
      {
        RemoveVectoredExceptionHandler (vectored_exception_handler_);
        vectored_exception_handler_ = nullptr;
      }

      // Restore original signal handlers.
      //
      if (crash_handler_installed_)
      {
        signal (SIGABRT, original_sigabrt_handler_);
        signal (SIGFPE, original_sigfpe_handler_);
        signal (SIGILL, original_sigill_handler_);
        signal (SIGINT, original_sigint_handler_);
        signal (SIGSEGV, original_sigsegv_handler_);
        signal (SIGTERM, original_sigterm_handler_);

        // Restore original exception filter.
        //
        SetUnhandledExceptionFilter (original_exception_filter_);
      }

      // Restore original terminate handler.
      //
      if (terminate_handler_installed_)
      {
        set_terminate (original_terminate_handler_);
        terminate_handler_installed_ = false;
      }

      crash_handler_installed_ = false;
    }

    void
    print_stacktrace (ostream &os, size_t skip_frames, const string &prefix)
    {
      stacktrace st;
      st.capture (skip_frames + 1); // +1 to starts at the caller.
      st.print (os, prefix);
    }
  }
}
