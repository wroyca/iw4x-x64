#pragma once

#include <memory>
#include <ostream>
#include <string>

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

#include <boost/locale.hpp>

namespace iw4x
{
  namespace utility
  {
    // Attaches the calling process to the console of the specified process
    // as a client application.
    //
    // chromium/src/base/process/launch_win.cc
    //
    void
    attach_console ();

    // A small wrapper around `CommandLineToArgvW()` for accessing arguments on
    // Windows outside of `main()`.
    //
    // Unlike on POSIX, Windows provides no stable `argv` array: each call to
    // `CommandLineToArgvW()` yields a fresh buffer that must be freed with
    // `LocalFree()`. This puts us in the business of tracking allocations we
    // never asked for, which is both easy to get wrong and awkwardly at odds
    // with the usual C++ ownership model.
    //
    // A second complication is that the Windows API stores arguments in UTF-16.
    // While defensible in principle, in practice the surrounding C++ code
    // almost always assumes UTF-8. Rather than force every call site to scatter
    // ad-hoc conversions, we centralize the matter here and present a UTF-8
    // façade.
    //
    // In short, this type makes two policy decisions:
    //
    //   1. Own the `CommandLineToArgvW()` buffer with proper RAII semantics, so
    //      resource management follows C++ expectations rather than Windows
    //      footnotes.
    //
    //   2. Translate arguments from UTF-16 to UTF-8 up front, so call sites
    //      never pay the cost of remembering which encoding they are in. The
    //      interface speaks UTF-8 because the language around it does.
    //
    // TODO: Replace with https://isocpp.org/files/papers/P3474R0.html if it
    // ever becomes accepted and/or an implementation is made available (for
    // example, via Boost).
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
          if (p != nullptr)
          {
            ::LocalFree (p);
          }
        }
      };

      using argv = std::unique_ptr<wchar_t *[], local_free_deleter>;
      using argc = int;
      using args = std::vector<std::string>;
      using argp = std::vector<char *>;

      // Construct by capturing the command line once, exactly as Windows parses
      // it. We make no attempt to reinterpret or override the OS' splitting
      // rules; they are treated as authoritative.
      //
      arguments_t ()
        : argc_ (0),
          argv_ (CommandLineToArgvW (GetCommandLineW (), &argc_))
      {
        argc_ = argv_ ? argc_ : 0;

        // All conversions to UTF-8 are performed eagerly. Arguments are
        // immutable for the process lifetime, so it's cleaner and cheaper to
        // cache the results once than to redo work on every access. The
        // interface exposed is therefore in UTF-8 only: the UTF-16 origin is an
        // implementation detail.
        //
        args_.reserve (static_cast<size_t> (argc_)),
          std::transform (argv_.get (),
                          argv_.get () + argc_,
                          std::back_inserter (args_),
                          [] (const wchar_t *w) -> std::string
        {
          // Null entries should not escape as undefined behavior. We normalize
          // them into empty strings so every slot yields a value. This avoids
          // exposing call sites to irregularities they are not equipped to
          // handle.
          //
          return w != nullptr
            ? boost::locale::conv::utf_to_utf<std::string::value_type> (w)
            : std::string ();
        });

        // Traditional `char**` array for compatibility with C-style interfaces.
        // Note that pointers here alias `args_` storage; they are never
        // owned independently.
        //
        argp_.reserve (static_cast<size_t> (argc_)),
          std::transform (args_.begin (),
                          args_.end (),
                          std::back_inserter (argp_),
                          [] (const std::string &s)
        {
          return const_cast<char *> (s.c_str ());
        });
      }

      ~arguments_t () = default;

      // Copy construction here is a deliberately narrow feature. Rather than
      // duplicating the entire buffer and cache, which would be expensive and
      // semantically dubious, we preserve only the count. The rationale is
      // ergonomic: many codebases are written with the `argc, argv` dual in
      // mind, and it is useful to mimic that convention without carrying the
      // full baggage of duplicated state.
      //
      // The absence of a deep copy is not an oversight but a signal:
      // duplicating Windows' internal allocation is neither practical nor
      // meaningful. Better to expose that reality directly than to pretend at
      // equivalence we cannot guarantee. Thus, copy means "count only," a
      // relaxation of the usual "copy means equality" rule.
      //
      arguments_t (const arguments_t &o) noexcept
        : argc_ (o.argc_)
      {
      }

      arguments_t &operator= (const arguments_t &) = delete;
      arguments_t (arguments_t &&) = delete;
      arguments_t &operator= (arguments_t &&) = delete;

      operator int () const noexcept
      {
        return argc_;
      }

      operator int & () noexcept
      {
        return argc_;
      }

      operator char ** () const
      {
        return argp_.data ();
      }

      operator const char * const * () const
      {
        return const_cast<const char *const *> (argp_.data ());
      }

      const char *
      operator[] (int i)
      {
        if (i < 0 || i >= argc_)
          throw std::out_of_range ("argc index out of range");

        return args_ [static_cast<size_t> (i)].c_str ();
      }

      friend std::ostream &
      operator<< (std::ostream &os, const arguments_t &a)
      {
        return os << a.argc_;
      }

    private:
      argc argc_;
      argv argv_;
      args args_;
      argp mutable argp_;
    };
  }
}
