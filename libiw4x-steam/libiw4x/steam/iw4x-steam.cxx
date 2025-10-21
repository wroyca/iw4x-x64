#include <libiw4x/steam/iw4x-steam.hxx>

#include <ostream>
#include <stdexcept>

using namespace std;

namespace iw4x
{
  namespace steam
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
