#include <sstream>
#include <stdexcept>

#include <libiw4x/discord/iw4x-discord.hxx>
#include <libiw4x/discord/version.hxx>

#undef NDEBUG
#include <cassert>

int
main ()
{
  using namespace std;
  using namespace iw4x::discord;

  // Basics.
  //
  {
    ostringstream o;
    say_hello (o, "World");
    assert (o.str () == "Hello, World!\n");
  }

  // Empty name.
  //
  try
  {
    ostringstream o;
    say_hello (o, "");
    assert (false);
  }
  catch (const invalid_argument& e)
  {
    assert (e.what () == string ("empty name"));
  }
}
