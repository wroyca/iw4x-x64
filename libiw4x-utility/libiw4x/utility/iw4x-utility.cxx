#include <libiw4x/utility/iw4x-utility.hxx>

#include <ostream>
#include <stdexcept>

using namespace std;

namespace iw4x_utility
{
  void say_hello (ostream& o, const string& n)
  {
    if (n.empty ())
      throw invalid_argument ("empty name");

    o << "Hello, " << n << '!' << endl;
  }
}
