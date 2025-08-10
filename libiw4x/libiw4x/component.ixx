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

  template <typename T> inline details::component_instance<T> &component<T>::
  instance_holder () noexcept
  {
    static details::component_instance<T> instance;
    return instance;
  }
}
