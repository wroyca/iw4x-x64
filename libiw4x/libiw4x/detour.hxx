#pragma once

#include <exception>
#include <ostream>

#include <libiw4x/export.hxx>

namespace iw4x
{
  namespace detour
  {
    class LIBIW4X_SYMEXPORT exception : public std::exception
    {
    public:
      virtual void
      print (std::ostream &) const = 0;
    };
  }
}
