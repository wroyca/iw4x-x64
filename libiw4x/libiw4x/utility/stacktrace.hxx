#pragma once

#include <string>
#include <iosfwd>
#include <cstddef>

#include <libiw4x/export.hxx>

namespace iw4x
{
  namespace utility
  {
    class LIBIW4X_SYMEXPORT stacktrace
    {
    public:
      stacktrace ();
      ~stacktrace ();

      // Non-copyable but movable.
      //
      stacktrace (const stacktrace &) = delete;
      stacktrace &operator= (const stacktrace &) = delete;

      stacktrace (stacktrace &&);
      stacktrace &operator= (stacktrace &&);

      // Capture the current call stack.
      //
      // This function captures the call stack at the point where it's called.
      // The number of frames to capture can be limited using the max_frames
      // parameter. A value of 0 means capture all available frames.
      //
      void
      capture (std::size_t max_frames = 0);

      // Capture the call stack skipping the specified number of frames.
      //
      // This is useful when you want to exclude wrapper functions from
      // the captured stack trace. For example, if this is called from
      // an error handler, you might want to skip the handler function
      // itself.
      //
      void
      capture (std::size_t skip_frames, std::size_t max_frames);

      // Check if a stack trace has been captured.
      //
      bool
      empty () const;

      // Get the number of captured frames.
      //
      std::size_t
      size () const;

      // Clear the captured stack trace.
      //
      void
      clear ();

      // Get a string representation of the stack trace.
      //
      // The format includes function names, file locations, and addresses
      // when available. Each frame is on a separate line.
      //
      std::string
      to_string () const;

      // Print the stack trace to the specified output stream.
      //
      // This is more efficient than calling to_string() when you just want
      // to output the stack trace, as it avoids creating an intermediate
      // string object.
      //
      void
      print (std::ostream &os) const;

      // Print the stack trace to the specified output stream with a prefix.
      //
      // The prefix is added to each line of the output. This is useful
      // for formatting or adding context information.
      //
      void
      print (std::ostream &os, const std::string &prefix) const;

    private:
      void *trace_; // Opaque pointer to boost::stacktrace::stacktrace
    };

    // Stream output.
    //
    LIBIW4X_SYMEXPORT std::ostream &
    operator<< (std::ostream &os, const stacktrace &st);

    // Install a crash handler that prints stack trace on termination.
    //
    // Note: This function is not thread-safe and should be called during
    // program initialization.
    //
    LIBIW4X_SYMEXPORT void
    install_crash_handler ();

    // Install a terminate handler that prints stack trace.
    //
    // Note: This function is not thread-safe and should be called during
    // program initialization.
    //
    LIBIW4X_SYMEXPORT void
    install_terminate_handler ();

    // Remove installed crash and terminate handlers.
    //
    // Note: This function is not thread-safe and should be called
    // during program cleanup or shutdown.
    //
    LIBIW4X_SYMEXPORT void
    remove_crash_handlers ();

    // Print the current stack trace to the specified output stream.
    //
    LIBIW4X_SYMEXPORT void
    print_stacktrace (std::ostream &os,
                      std::size_t skip_frames = 0,
                      const std::string &prefix = "");
  }
}
