#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <future>
#include <chrono>

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
      : initialized_ (true)
    {
      auto& basic = basic_component::get ();
      initial_value_ = basic.get_value ();
    }

    bool
    is_initialized () const
    {
      return initialized_;
    }

    int
    get_initial_value () const
    {
      return initial_value_;
    }

    static std::vector<std::type_index>
    component_dependencies ()
    {
      return {std::type_index (typeid (basic_component))};
    }

  private:
    bool initialized_ {};
    int initial_value_;
  };

  class complex_component final : public component<complex_component>
  {
  public:
    complex_component ()
    {
      auto& basic = basic_component::get ();
      auto& dependency = dependency_component::get ();

      count_ = basic.get_value () + (dependency.is_initialized () ? 10 : 0);
      basic_ref_value_ = basic.get_value ();
      dependency_init_value_ = dependency.get_initial_value ();
    }

    void
    increment ()
    {
      ++count_;
    }

    int
    get_count () const
    {
      return count_;
    }

    int
    get_basic_ref_value () const
    {
      return basic_ref_value_;
    }

    int
    get_dependency_init_value () const
    {
      return dependency_init_value_;
    }

    static std::vector<std::type_index>
    component_dependencies ()
    {
      return {
          std::type_index (typeid (basic_component)),
          std::type_index (typeid (dependency_component))
      };
    }

  private:
    int count_ {};
    int basic_ref_value_ {};
    int dependency_init_value_ {};
  };

  class thread_safe_component final : public component<thread_safe_component>
  {
  public:
    thread_safe_component () = default;

    void
    increment ()
    {
      std::lock_guard<std::mutex> lock (mutex_);
      ++counter_;
    }

    int
    get_counter () const
    {
      std::lock_guard<std::mutex> lock (mutex_);
      return counter_;
    }

  private:
    mutable std::mutex mutex_;
    int counter_ {};
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

      r.validate_dependencies ();
    }

    void
    test_dependency_injection ()
    {
      auto &r = component_registry::instance ();

      auto &basic = basic_component::get ();
      basic.set_value (200);

      r.initialize_all ();

      auto &dependency = dependency_component::get ();
      assert (dependency.is_initialized ());
      assert (dependency.get_initial_value () == 200);

      auto &complex = complex_component::get ();
      assert (complex.get_count () == 210);
      assert (complex.get_basic_ref_value () == 200);
      assert (complex.get_dependency_init_value () == 200);

      assert (complex.get_dependency_init_value () == dependency.get_initial_value ());

      complex.increment ();
      assert (complex.get_count () == 211);
    }

    void
    test_component_existance ()
    {
      assert (basic_component::exists ());
    }

    void
    test_thread_safety ()
    {
      auto &registry = component_registry::instance ();
      registry.register_singleton<thread_safe_component> ();

      const int num_threads = 10;
      const int increments_per_thread = 100;

      std::vector<std::future<void>> futures;

      for (int i = 0; i < num_threads; ++i)
      {
        futures.push_back (std::async (std::launch::async, [] ()
        {
          auto &comp = thread_safe_component::get ();
          for (int j = 0; j < increments_per_thread; ++j)
          {
            comp.increment ();
          }
        }));
      }

      for (auto &future : futures)
      {
        future.wait ();
      }

      auto &comp = thread_safe_component::get ();
      int expected = num_threads * increments_per_thread;
      assert (comp.get_counter () == expected);
    }
    void
    test_type_safety ()
    {
      auto basic_type = basic_component::type_id ();
      auto dependency_type = dependency_component::type_id ();

      assert (basic_type != dependency_type);
      assert (basic_type == std::type_index (typeid (basic_component)));
      assert (dependency_type == std::type_index (typeid (dependency_component)));
    }
  }
}

int main ()
{
  using namespace iw4x;

  test_basic_component ();
  test_component_registry ();
  test_dependency_injection ();
  test_component_existance ();
  test_type_safety ();
  test_thread_safety ();
}
