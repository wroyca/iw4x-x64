#include <libiw4x/imgui/imgui.hxx>

#include <ostream>
#include <stdexcept>

using namespace std;

namespace iw4x
{
  namespace imgui
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
