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

  namespace
  {
    void
    test_basic_component ()
    {
      //
      //
      auto& b1 (basic_component::get ());
      assert (b1.get_value () == 42);

      //
      //
      auto& b2 (basic_component::get ());
      b2.set_value (100);
      assert (b1.get_value () == 100);

      //
      //
      assert (&b1 == &b2);
    }

    void
    test_component_registry ()
    {
      auto &r (component_registry::instance ());

      r.register_singleton<basic_component> ();
      assert (r.is_registered<basic_component> ());
    }
  }
}

int main ()
{
  using namespace iw4x;

  test_basic_component ();
  test_component_registry ();
}
