#include <libiw4x/utility/utility.hxx>

#include <ostream>
#include <stdexcept>

using namespace std;

namespace iw4x
{
  namespace utility
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
