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
    std::vector<std::type_index> dependencies;

    // Note that for components without dependencies, the vector remains empty,
    // which is semantically correct: an empty dependency list means "no
    // dependencies", not "unknown dependencies".
    //
    if constexpr (details::has_dependencies<T>::value)
      dependencies = T::component_dependencies ();

    register_component_impl<T> (std::move (dependencies));
  }

  template <typename T, typename... Dependencies> inline void component_registry::
  register_singleton_with_dependencies ()
  {
    std::vector<std::type_index> d ({
      std::type_index (typeid (Dependencies))...
    });

    register_component_impl<T> (std::move (d));
  }

  template <typename T> inline bool component_registry::
  is_registered () const noexcept
  {
    auto& m (registry_mutex_);
    auto& c (registered_components_);

    // registration might be happening concurrently with queries.
    //
    std::lock_guard<std::mutex> lock (m);

    return c.contains (std::type_index (typeid (T)));
  }

  template <typename T> inline void component_registry::
  register_component_impl (std::vector<std::type_index> d)
  {
    auto& m (registry_mutex_);
    auto& c (registered_components_);
    auto& r (resolver_);

    // Even though the bulk of component registration tends to happen during
    // static initialization, runtime registration from multiple threads is
    // possible and must be handled. We serialize here to protect the
    // registry from concurrent mutations, but deliberately keep the
    // critical section narrow to avoid holding the mutex across potentially
    // long-running work.
    //
    std::lock_guard<std::mutex> lock (m);

    // We do not want to error out if the same component type is registered more
    // than once. Such case is common with layered initialization and can occur
    // harmlessly. The only real concern is avoiding multiple resolver
    // registrations for the same type, so we check our own set first and we
    // silently skip it if its already present.
    //
    std::type_index component_id (std::type_index (typeid (T)));
    if (c.contains (component_id))
      return;

    // Registering with the dependency resolver allows it to compute an
    // initialization order that satisfies all dependencies without us needing
    // to preemptively construct the component. We deliberately defer the actual
    // instantiation by passing a lambda that captures the type but does nothing
    // until the resolver decides the time is right.
    //
    r.register_component (
      component_id,
      [] () { component<T>::get (); },
      std::move (d));

    c.insert (component_id);
  }
}
