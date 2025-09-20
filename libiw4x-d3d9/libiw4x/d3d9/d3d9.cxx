#include <libiw4x/d3d9/d3d9.hxx>

#include <ostream>
#include <stdexcept>

using namespace std;

namespace iw4x
{
  namespace d3d9
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
