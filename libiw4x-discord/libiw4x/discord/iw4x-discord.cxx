#include <libiw4x/discord/iw4x-discord.hxx>

#include <ostream>
#include <stdexcept>

#include <discordpp.h>

using namespace std;

namespace iw4x
{
  namespace discord
  {
    void
    say_hello (ostream& o, const string& n)
    {
      if (n.empty ())
        throw invalid_argument ("empty name");

      o << "Hello, " << n << '!' << endl;
    }
  }
}
