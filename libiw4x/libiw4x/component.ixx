#pragma once

namespace iw4x
{
  namespace details
  {
    template <typename T>
    inline component_instance<T>::pointer_type
    component_instance<T>::get_or_create ()
    {
      T *instance = instance_.load (std::memory_order_acquire);

      if (instance == nullptr)
      {
        std::call_once (init_flag_, [this] ()
        {
          shared_instance_ = std::make_shared<T> ();
          instance_.store (shared_instance_.get (), std::memory_order_release);
        });

        instance = instance_.load (std::memory_order_acquire);
      }

      return shared_instance_;
    }
  }

  template <typename T> inline T &component<T>::
  get ()
  {
    return *instance_holder ().get_or_create ();
  }

  template <typename T> inline bool component<T>::
  exists () noexcept
  {
    return instance_holder ().exists ();
  }

  template <typename T> inline details::component_instance<T> &component<T>::
  instance_holder () noexcept
  {
    static details::component_instance<T> instance;
    return instance;
  }

  template <typename T> inline void component_registry::
  register_singleton ()
  {
    std::vector<std::type_index> deps;

    if constexpr (details::has_dependencies<T>::value)
      deps = T::component_dependencies ();

    register_component_impl<T> (std::move (deps));
  }

  template <typename T, typename... Dependencies> inline void component_registry::
  register_singleton_with_deps ()
  {
    std::vector<std::type_index> d = {std::type_index (typeid (Dependencies))...};
    register_component_impl<T> (std::move (d));
  }

  template <typename T> inline bool component_registry::
  is_registered () const noexcept
  {
    std::lock_guard<std::mutex> lock (registry_mutex_);

    return registered_components_.contains (std::type_index (typeid (T)));
  }

  template <typename T> inline void component_registry::
  register_component_impl (std::vector<std::type_index> d)
  {
    std::lock_guard<std::mutex> lock (registry_mutex_);

    auto component_id (std::type_index (typeid (T)));

    if (registered_components_.contains (component_id))
      return;

    auto init_func = [] ()
    {
      component<T>::get ();
    };

    resolver_.register_component (component_id, init_func, std::move (d));
    registered_components_.insert (component_id);
  }
}
