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

    bool dependency_resolver::
    validate_dependencies () const
    {
      std::unordered_set<component_id> visited;
      std::unordered_set<component_id> visiting;

      try
      {
        for (const auto& [id, info] : components_)
        {
          if (visited.find (id) == visited.end ())
          {
            check_cycles (id, visiting, visited);
          }
        }
        return true;
      }
      catch (const std::runtime_error&)
      {
        throw; // XXX: TODO
      }
    }

    void dependency_resolver::
    check_cycles (component_id id,
                  std::unordered_set<component_id>& visiting,
                  std::unordered_set<component_id>& visited) const
    {
      if (visiting.contains (id))
        throw std::runtime_error ("dependency cycle detected");

      if (visited.contains (id))
        return;

      visiting.insert (id);

      auto it = components_.find (id);
      if (it != components_.end ())
      {
        for (const auto &d : it->second.dependencies)
        {
          check_cycles (d, visiting, visited);
        }
      }

      visiting.erase (id);
      visited.insert (id);
    }
  }

  component_registry &component_registry::
  instance () noexcept
  {
    static component_registry instance_;
    return instance_;
  }

  bool component_registry::
  validate_dependencies () const
  {
    return resolver_.validate_dependencies ();
  }
}
