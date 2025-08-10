#pragma once

namespace iw4x
{
  namespace details
  {
    template <typename T> inline component_instance<T>::pointer_type component_instance<T>::
    get_or_create ()
    {
      auto& r (instance_);
      auto& s (shared_instance_);

      // The steady-state case is the common one: the component has already
      // been initialized, and all we need is to return a reference to it. We
      // therefore attempt a lock-free read of the published raw pointer first.
      //
      T *instance (r.load (std::memory_order_acquire));

      // If it is non-null, then we know the object is fully constructed and
      // visible due to the acquire semantics on the load.
      //
      if (instance != nullptr)
        return s;

      // If we get here, then we are on a slow path: the instance does not yet
      // exist, or at least has not been published to us.
      //
      // Note that multiple threads may arrive here simultaneously, but only
      // one of them should actually perform the initialization.
      //
      std::call_once (init_flag_,
                      [this, &r, &s] ()
      {
        // We deliberately construct the shared instance before publishing its
        // raw pointer. This guarantees that any thread which subsequently
        // reads the pointer will observe a fully-formed object.
        //
        s = std::make_shared<T> ();

        // The release semantics here are not optional. They form the "publish"
        // half of the acquire/release handshake that makes the initial
        // fast-path load reliable. Without the release, another thread could
        // see a non-null pointer but still observe a partially-initialized
        // object.
        //
        r.store (s.get (), std::memory_order_release);
      });

      return s;
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
