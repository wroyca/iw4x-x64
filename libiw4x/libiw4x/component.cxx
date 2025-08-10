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

    void dependency_resolver::
    initialize_all ()
    {
      std::lock_guard<std::mutex> lock (mutex_);

      validate_dependencies ();

      for (auto &[id, info] : components_)
      {
        if (!info.initialized)
        {
          initialize_component (id);
        }
      }
    }

    void dependency_resolver::
    initialize_component (component_id id)
    {
      auto it = components_.find (id);
      if (it == components_.end ())
        throw std::runtime_error ("component not registered");

      component_info& info = it->second;
      if (info.initialized)
        return;

      for (const auto& dep_id : info.dependencies)
        initialize_component (dep_id);

      if (info.init)
        info.init ();

      info.initialized = true;
    }

    void dependency_resolver::
    validate_dependencies () const
    {
      std::unordered_set<component_id> visited;
      std::unordered_set<component_id> visiting;

      try
      {
        for (const auto& [id, info] : components_)
        {
          if (!visited.contains (id))
            check_cycles (id, visiting, visited);
        }
      }
      catch (const std::runtime_error&)
      {
        throw std::runtime_error ("component dependency cycle detected");
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

  void component_registry::
  initialize_all ()
  {
    resolver_.initialize_all ();
  }

  void component_registry::
  validate_dependencies () const
  {
    resolver_.validate_dependencies ();
  }
}
