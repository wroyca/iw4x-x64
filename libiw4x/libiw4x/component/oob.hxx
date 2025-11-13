#pragma once

#include <string>
#include <unordered_map>

#include <libiw4x/types.hxx>

#include <libiw4x/utility/scheduler.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  class LIBIW4X_SYMEXPORT oob
  {
    using handler = std::function<void (const netadr_t&, const std::vector<std::string>&)>;

  public:
    explicit
    oob (utility::scheduler&);

    bool
    dispatch (const netadr_t&, const msg_t&);

    void
    register_handler (const std::string&, handler);

  private:
    std::unordered_map<std::string, handler> handlers_;
  };
}
