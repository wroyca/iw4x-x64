#pragma once

namespace iw4x
{
  template <typename T>
  class component
  {
  public:
    using component_type = T;

  protected:
    component () = default;
  };
}
