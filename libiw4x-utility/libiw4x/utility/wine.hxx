#pragma once

#include <compare>
#include <cstdint>
#include <expected>
#include <format>
#include <functional>
#include <stdexcept>
#include <string>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  namespace utility
  {
    // Wine version parse error.
    //
    class LIBIW4X_UTILITY_SYMEXPORT wine_version_parse_error
      : public std::runtime_error
    {
    public:
      explicit wine_version_parse_error (const std::string& what_arg)
        : std::runtime_error (what_arg)
      {
      }

      explicit wine_version_parse_error (const char* what_arg)
        : std::runtime_error (what_arg)
      {
      }
    };

    // Wine version detection and parsing.
    //
    // Wine version format: <major>.<minor>[.<patch>][-rc<N>] [<build-info>]
    //
    // Examples:
    //   9.0
    //   8.21
    //   9.0-rc1
    //   8.0.2
    //   wine-9.0 (Staging)
    //
    struct LIBIW4X_UTILITY_SYMEXPORT wine_version
    {
      std::uint64_t major = 0;
      std::uint64_t minor = 0;
      std::uint64_t patch = 0;
      std::uint64_t rc = 0;
      std::string build_info;

      wine_version () = default;
      wine_version (std::uint64_t major,
                    std::uint64_t minor,
                    std::uint64_t patch = 0,
                    std::uint64_t rc = 0,
                    std::string build_info = "");

      // Parse Wine version string.
      //
      // Throws std::invalid_argument if the format is not recognizable.
      //
      explicit wine_version (const std::string& version);

      // Convert wine version to string representation.
      //
      // Note that patch version is omitted if it is 0, RC version is omitted if
      // it is 0, and build info is omitted if empty. The "wine-" prefix is
      // never included in the output.
      //
      std::string
      to_string () const;

      // Three-way comparison (ignores build_info).
      //
      // Note: RC versions are considered less than stable releases.
      // For example: 9.0-rc1 < 9.0
      //
      std::strong_ordering
      operator<=> (const wine_version& v) const
      {
        if (std::strong_ordering cmp = major <=> v.major; cmp != 0)
          return cmp;

        if (std::strong_ordering cmp = minor <=> v.minor; cmp != 0)
          return cmp;

        if (std::strong_ordering cmp = patch <=> v.patch; cmp != 0)
          return cmp;

        // If one is RC and the other is stable, stable is greater
        //
        if (rc == 0 && v.rc != 0)
          return std::strong_ordering::greater;

        if (rc != 0 && v.rc == 0)
          return std::strong_ordering::less;

        // Both are RC or both are stable
        //
        return rc <=> v.rc;
      }

      // Equality operator (ignores build_info, consistent with operator<=> ()).
      //
      bool
      operator== (const wine_version& v) const = default;
    };

    // Try to parse a Wine version string.
    //
    // Returns error if the string is not a valid Wine version.
    //
    LIBIW4X_UTILITY_SYMEXPORT std::expected<wine_version, wine_version_parse_error>
    wine_parse_version (const std::string& version);
  }
}

// std::formatter specialization for wine_version.
//
template <>
struct std::formatter<iw4x::utility::wine_version, char>
{
  constexpr auto
  parse (format_parse_context& ctx)
  {
    return ctx.begin ();
  }

  auto
  format (const iw4x::utility::wine_version& v, format_context& ctx) const
  {
    return format_to (ctx.out (), "{}", v.to_string());
  }
};
