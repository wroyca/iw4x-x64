#pragma once

#include <type_traits>

#include <libiw4x/export.hxx>

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

    // Get component instance (singleton access).
    //
    static T&
    get ();

  protected:
    component () = default;
  };
}

#include <libiw4x/component.ixx>
