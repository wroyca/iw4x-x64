#pragma once

#include <libiw4x/utility/scheduler.hxx>

#include <libiw4x/mod/oob.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  namespace mod
  {
    class LIBIW4X_SYMEXPORT party
    {
    public:
      explicit
      party (oob&);

      void
      connect_to_party (netadr_t& address);
    };
  }
}
