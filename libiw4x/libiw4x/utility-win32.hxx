#pragma once

#include <ios>
#include <memory>

#include <boost/locale.hpp>

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

#include <shellapi.h>
#include <io.h>

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

  // A small wrapper around `CommandLineToArgvW()` for accessing arguments on
  // Windows outside of `main()`.
  //
  // Unlike on POSIX, Windows provides no stable `argv` array: each call to
  // `CommandLineToArgvW()` yields a fresh buffer that must be freed with
  // `LocalFree()`. This puts us in the business of tracking allocations we
  // never asked for, which is both easy to get wrong and awkwardly at odds with
  // the usual C++ ownership model.
  //
  // A second complication is that the Windows API stores arguments in UTF-16.
  // While defensible in principle, in practice the surrounding C++ code almost
  // always assumes UTF-8. Rather than force every call site to scatter ad-hoc
  // conversions, we centralize the matter here and present a UTF-8 façade.
  //
  // In short, this type makes two policy decisions:
  //
  //   1. Own the `CommandLineToArgvW()` buffer with proper RAII semantics, so
  //      resource management follows C++ expectations rather than Windows
  //      footnotes.
  //
  //   2. Perform UTF-16 -> UTF-8 conversion at the point of access, so callers
  //      operate in the more idiomatic encoding without incidental boilerplate.
  //
  // TODO: Replace with https://isocpp.org/files/papers/P3474R0.html if it ever
  // becomes accepted and/or an implementation is made available (for example,
  // via Boost). At that point this bespoke wrapper should be retired in favour
  // of the standard abstraction.
  //
  struct arguments_t
  {
    // We spell out the custom deleter as a type instead of burying it in a
    // lambda to keep the ownership rule visible where it matters. Caching
    // this knowledge in a helper header tends to age poorly: we stop seeing
    // it, and mistakes drift back in at the call sites.
    //
    struct local_free_deleter
    {
      void
      operator() (void *p) const noexcept
      {
        // The null check is not for correctness (Windows tolerates null) but
        // for intent: avoid paying a system call when the state is already
        // "nothing to do". Micro-cost aside, it signals that freeing may be a
        // hot-path event and should be cheap when vacuous.
        //
        if (p)
          ::LocalFree (p);
      }
    };

    std::unique_ptr<wchar_t *[], local_free_deleter> argv_;
    int size_ {0};

    // Default construction chooses "current process" because that is the only
    // stable source we can reasonably standardize on. Anything more general
    // would reopen things we do not want this type to arbitrate.
    //
    arguments_t ()
      : arguments_t (from_current_process ())
    {
    }

    // Ownership injection point. Kept explicit to discourage casual conversions
    // from unrelated pointers and to communicate that constructing this object
    // is an operation with ownership transfer semantics, not merely a view.
    //
    // Marked `noexcept` to document expectations: we are passing through a
    // system allocation and storing two trivially movable values; failure here
    // would indicate a broader program invariant violation rather than a
    // recoverable situation.
    //
    explicit arguments_t (std::unique_ptr<wchar_t *[], local_free_deleter> a,
                          int n) noexcept
      : argv_ (std::move (a)),
        size_ (n)
    {
    }

    // Copy constructor: count-only.
    //
    // The copy use case we care about is ergonomic: `auto argc (argv);` to
    // mirror conventional patterns in surrounding code. Duplicating the
    // buffer would be the wrong kind of convenience: expensive and still
    // unable to reproduce the system's exact allocation semantics. We
    // therefore copy the count and make the absence of data explicit.
    //
    // This is an intentional deviation from "copy means two equal objects".
    // Here, copy means "propagate the aspect callers actually want", which is
    // the count. The alternative is to forbid copy outright and force callers
    // to write it out manually; experience suggests that buys little while
    // inviting more boilerplate.
    //
    arguments_t (const arguments_t &o) noexcept
      : argv_ (nullptr),
        size_ (o.size_)
    {
    }

    arguments_t
    operator= (arguments_t &) = delete;

    // Portability gap across compilers: once we introduce a copy constructor,
    // the rules for whether a move constructor or move assignment operator is
    // still implicitly declared diverge. Some compilers (for example, Clang
    // in MinGW mode) continue to generate one, others suppress it. The net
    // result is not merely cosmetic but semantic: in the presence of an
    // implicitly declared move, temporaries may transfer `unique_ptr`
    // ownership rather than following our intended "count-only" semantics.
    //
    // In practice this means the buffer can be released (via `LocalFree`)
    // while we still expect `argv` entries to remain valid throughout
    // `main()`. The effect is subtle but catastrophic: pointers appear intact
    // at declaration, yet by first use they have evaporated to null.
    //
    arguments_t (arguments_t &&) = default;
    arguments_t &
    operator= (arguments_t &&) = delete;

    // Access policy: convert-on-demand.
    //
    // We convert UTF-16 -> UTF-8 at the point of access rather than up front.
    // The cost model is that accesses are typically sparse (a handful of
    // flags or paths), and forced eager conversion risks paying for strings
    // nobody reads. Deferring also keeps failure modes local: if a particular
    // argument is ill-formed, the exception (or error signaling, if the
    // converter uses one) is thrown where the value is actually consumed.
    //
    // Note that we use an `assert()` for bounds in debug builds. The intent
    // is to catch programming errors early without promoting index validation
    // to a runtime cost in paths that already know their indices.
    //
    std::string
    utf8_at (int i) const
    {
      assert (i >= 0 && i < size_);
      const wchar_t *w (argv_ ? argv_ [i] : nullptr);
      return w ? boost::locale::conv::utf_to_utf<char> (w) : std::string ();
    }

    // Implicit conversion to int encourages continuity with existing patterns
    // ("argc as an integer") without inventing a new method name for the same
    // concept. The usual concern with implicit conversions (i.e. loss of
    // clarity) does not apply here: the target type communicates the intent
    // unambiguously, and there is no ownership or lifetime hidden behind this
    // conversion.
    //
    operator int () const noexcept
    {
      return size_;
    }

    // Indexing preserves the familiar surface (`argv[i]`) while keeping the
    // UTF-8 policy centralized in `utf8_at()`. Note that we deliberately
    // avoid returning wide strings here to prevent accidental reintroduction
    // of UTF-16 into higher layers that are otherwise narrow-by-design.
    //
    std::string
    operator[] (int i) const
    {
      return utf8_at (i);
    }

    // Single choke point for parsing and ownership capture.
    //
    // Concentrating the `GetCommandLineW()`/`CommandLineToArgvW()` pairing
    // here avoids duplicating parsing/quoting knowledge and, more
    // importantly, centralizes the lifetime hand-off into one place. If
    // Windows ever changes quoting rules or we need to interpose additional
    // validation, there is a single site to adjust.
    //
    static arguments_t
    from_current_process () noexcept
    {
      int n (0);
      std::unique_ptr<wchar_t *[], local_free_deleter> a (
        CommandLineToArgvW (GetCommandLineW (), &n));

      return arguments_t {std::move (a), n};
    }
  };
}

