#pragma once

namespace iw4x
{
  namespace details
  {
    template <typename T>
    struct component_traits
    {
      using type = T;
    };
  }

  template <typename T>
  class component
  {
  public:
    using component_type = T;
    using traits_type = details::component_traits<T>;

  protected:
    component () = default;
  };
}
