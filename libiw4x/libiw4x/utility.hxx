#pragma once

namespace iw4x
{
  template <typename F, typename P>
  inline F
  function_cast (P *p)
  {
    union
    {
      P *p;
      F f;
    } r;

    r.p = p;
    return r.f;
  }
}
