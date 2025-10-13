#include <libiw4x/utility/utility-win32.hxx>

#undef NDEBUG
#include <cassert>

int main ()
{
  using namespace std;
  using namespace iw4x::utility;

  // Test file descriptor state before and after attach
  //
  {
    int stdout_fd_before = _fileno (stdout);
    int stderr_fd_before = _fileno (stderr);

    bool stdout_valid_before = stdout_fd_before >= 0;
    bool stderr_valid_before = stderr_fd_before >= 0;

    attach_console ();

    int stdout_fd_after = _fileno (stdout);
    int stderr_fd_after = _fileno (stderr);

    // Streams should remain valid (or become valid).
    //
    assert (stdout_fd_after >= 0 || stdout_fd_after == -2);
    assert (stderr_fd_after >= 0 || stderr_fd_after == -2);
  }
}
