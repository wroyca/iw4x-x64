#pragma once

#include <iosfwd>
#include <string>

#include <libiw4x/discord/export.hxx>

namespace iw4x_discord
{
  // Print a greeting for the specified name into the specified
  // stream. Throw std::invalid_argument if the name is empty.
  //
  LIBIW4X_DISCORD_SYMEXPORT void
  say_hello (std::ostream&, const std::string& name);
}
