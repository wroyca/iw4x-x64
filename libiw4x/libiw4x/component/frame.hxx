#pragma once

#include <libiw4x/utility/scheduler.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  class LIBIW4X_SYMEXPORT frame
  {
  public:
    explicit
    frame (utility::scheduler& scheduler_instance);
  };
}
