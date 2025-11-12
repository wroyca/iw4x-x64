#include <string>

#include <libiw4x/utility/utility-win32.hxx>

#undef NDEBUG
#include <cassert>

int main ()
{
  using namespace std;
  using namespace iw4x::utility;

  // Test format_message with well-known Windows error codes
  //
  {
    // ERROR_SUCCESS (0)
    //
    {
      string msg (format_message (0));
      assert (!msg.empty ());
    }

    // ERROR_FILE_NOT_FOUND (2)
    //
    {
      string msg (format_message (2));
      assert (!msg.empty ());
    }

    // ERROR_ACCESS_DENIED (5)
    //
    {
      string msg (format_message (5));
      assert (!msg.empty ());
    }
  }

  // Test that unknown error code return fallback message
  //
  {
    string msg (format_message (999999999));
    assert (msg == "unknown error code 999999999");
  }

  // Test that messages don't contain trailing whitespace/newlines
  //
  {
    string msg (format_message (2));
    assert (!msg.empty () && msg.back () != '\n' && msg.back () != '\r');
  }
}
