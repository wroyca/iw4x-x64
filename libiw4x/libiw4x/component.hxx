#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <type_traits>

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
  };
}

#include <libiw4x/component.ixx>
