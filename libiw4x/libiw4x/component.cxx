#include <libiw4x/component.hxx>

namespace iw4x
{
  namespace details
  {
    void dependency_resolver::
    register_component (component_id id,
                        init_function init,
                        std::vector<component_id> d)
    {
      std::lock_guard<std::mutex> lock (mutex_);

      components_[id] = component_info{std::move (init), std::move (d), false};
    }
  }

  component_registry &component_registry::
  instance () noexcept
  {
    static component_registry instance_;
    return instance_;
  }
}
