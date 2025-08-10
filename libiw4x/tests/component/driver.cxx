#include <sstream>
#include <stdexcept>

#include <libiw4x/version.hxx>
#include <libiw4x/component.hxx>

#undef NDEBUG
#include <cassert>

using namespace std;

namespace iw4x
{
  class basic_component final : public component<basic_component>
  {
  public:
    basic_component ()
      : value_ (42) {}

    int
    get_value () const { return value_; }

    void
    set_value (int v) { value_ = v; }

  private:
    int value_ {};
  };
}

int main ()
{
  using namespace iw4x;
}
