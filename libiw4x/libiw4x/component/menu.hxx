#pragma once

#include <libiw4x/database.hxx>

#include <libiw4x/utility/scheduler.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  class LIBIW4X_SYMEXPORT menu
  {
  public:
    explicit
    menu (utility::scheduler& scheduler_instance);
  };
}
