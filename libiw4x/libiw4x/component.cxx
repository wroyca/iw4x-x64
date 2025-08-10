#include <libiw4x/component.hxx>

namespace iw4x
{
  component_registry &component_registry::
  instance () noexcept
  {
    static component_registry instance_;
    return instance_;
  }
}
