#pragma once


#include <atomic>
#include <memory>
#include <mutex>
#include <type_traits>
#include <typeindex>
#include <vector>
#include <unordered_set>
#include <functional>

#include <libiw4x/export.hxx>

namespace iw4x
{
  namespace details
  {
    template <typename T>
    struct component_traits
    {
      using type = T;
    };

    // clang-format off
    template <typename T, typename = void>
    struct has_dependencies : std::false_type {};

    template <typename T>
    struct has_dependencies<T, std::enable_if_t<
      std::is_same_v<
        decltype (T::component_dependencies ()),
        std::vector<std::type_index>
      >
    >> : std::true_type {};
    // clang-format on

    template <typename T>
    class component_instance
    {
    public:
      using instance_type = T;
      using pointer_type = std::shared_ptr<T>;

      // Instance access with lazy initialization.
      //
      pointer_type
      get_or_create ();

    private:
      mutable std::atomic<T *> instance_ {nullptr};
      mutable pointer_type shared_instance_;
      mutable std::once_flag  init_flag_;
    };

    class LIBIW4X_SYMEXPORT dependency_resolver
    {
    public:
      using component_id = std::type_index;
      using init_function = std::function<void()>;

      void
      register_component (component_id id,
                          init_function init,
                          std::vector<component_id> dependencies = {});

    private:
      struct component_info
      {
        init_function init;
        std::vector<component_id> dependencies;
        bool initialized = false;
      };

      std::unordered_map<component_id, component_info> components_;
      mutable std::mutex mutex_;
    };
  }

  template <typename T>
  class component
  {
  public:
    using component_type = T;
    using traits_type = details::component_traits<T>;

    // Get component instance (singleton access).
    //
    static T&
    get ();

    // Component dependencies (override in derived class if needed).
    //
    static std::vector<std::type_index>
    component_dependencies () { return {}; }

  protected:
    component () = default;

  private:
    friend class component_registry;

    static details::component_instance<T> &
    instance_holder () noexcept;
  };

  class LIBIW4X_SYMEXPORT component_registry
  {
  public:
    static component_registry &
    instance () noexcept;

    // Register a component as a global singleton.
    //
    template <typename T> void
    register_singleton ();

    template <typename T> bool
    is_registered () const noexcept;

  private:
    mutable std::mutex registry_mutex_;
    std::unordered_set<std::type_index> registered_components_;
    details::dependency_resolver resolver_;

    template <typename T>
    void register_component_impl (std::vector<std::type_index> dependencies);
  };
}

#include <libiw4x/component.ixx>
