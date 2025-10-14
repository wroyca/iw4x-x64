#include <libiw4x/utility/utility-win32.hxx>

#include <memory>

using namespace std;

namespace iw4x
{
  namespace utility
  {
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
