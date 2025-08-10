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

  class dependency_component final : public component<dependency_component>
  {
  public:
    dependency_component ()
      : initialized_ (true) {}

    bool
    is_initialized () const
    {
      return initialized_;
    }

    static std::vector<std::type_index>
    component_dependencies ()
    {
      return {std::type_index (typeid (basic_component))};
    }

  private:
    bool initialized_ {};
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

      r.register_singleton<dependency_component> ();
      assert (r.is_registered<dependency_component> ());

      assert (r.validate_dependencies ());
    }
  }
}

int main ()
{
  using namespace iw4x;

  test_basic_component ();
  test_component_registry ();
}
