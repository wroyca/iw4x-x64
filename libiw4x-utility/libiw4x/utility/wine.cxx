#include <libiw4x/utility/wine.hxx>

#include <algorithm>
#include <stdexcept>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    namespace
    {
      // Parse unsigned integer from string.
      //
      // Parses a sequence of digits starting at position p in string s and
      // stores the result in r. Updates p to point to the first character
      // after the parsed number.
      //
      // Returns false if no digits are found at position p or if the number
      // would overflow uint64_t. Returns true on success.
      //
      bool
      parse_uint64 (const string& s, size_t& p, uint64_t& r)
      {
        // Check if we have at least one digit at current position
        //
        if (p >= s.size () || !isdigit (static_cast<unsigned char> (s [p])))
          return false;

        size_t i (p);
        r = 0;

        // Parse digits and accumulate result
        //
        for (; i < s.size () && isdigit (static_cast<unsigned char> (s [i]));
             ++i)
        {
          uint64_t d (s [i] - '0');

          // Check for overflow
          //
          if (r > (UINT64_MAX - d) / 10)
            return false;

          r = r * 10 + d;
        }

        // Update position to point past the parsed number
        //
        p = i;
        return true;
      }
    }

    wine_version::wine_version (const string& s)
    {
      auto v = wine_parse_version (s);

      if (!v)
        throw invalid_argument ("invalid wine version: " + s);

      *this = *v;
    }

    string
    wine_version::to_string () const
    {
      string r;

      // Format major.minor (always present)
      //
      r += std::to_string (major);
      r += '.';
      r += std::to_string (minor);

      // Add optional patch version
      //
      if (patch != 0)
      {
        r += '.';
        r += std::to_string (patch);
      }

      // Add optional RC version
      //
      if (rc != 0)
      {
        r += "-rc";
        r += std::to_string (rc);
      }

      // Add optional build info
      //
      if (!build_info.empty ())
      {
        r += ' ';
        r += build_info;
      }

      return r;
    }

    expected<wine_version, wine_version_parse_error>
    wine_parse_version (const string& s)
    {
      using e = unexpected<wine_version_parse_error>;

      wine_version r;
      size_t p (0);

      // Skip "wine-" prefix if present
      //
      if (s.compare (0, 5, "wine-") == 0)
        p = 5;

      // Parse major version
      //
      if (!parse_uint64 (s, p, r.major))
        return e ("invalid major version");

      // Parse minor version
      //
      if (p >= s.size () || s [p] != '.')
        return e ("missing or invalid minor version");

      ++p;

      if (!parse_uint64 (s, p, r.minor))
        return e ("invalid minor version");

      // Parse optional patch version
      //
      if (p < s.size () && s [p] == '.')
      {
        ++p;

        if (!parse_uint64 (s, p, r.patch))
          return e ("invalid patch version");
      }

      // Parse optional RC version
      //
      if (p < s.size () && s [p] == '-')
      {
        ++p;

        // Check for "rc" prefix
        //
        if (s.compare (p, 2, "rc") != 0)
          return e ("invalid RC format");

        p += 2;

        if (!parse_uint64 (s, p, r.rc))
          return e ("invalid RC version");
      }

      // Everything remaining is build info
      //
      if (p < s.size ())
      {
        // Build info must start with whitespace
        //
        if (!isspace (static_cast<unsigned char> (s [p])))
          return e ("invalid build info format");

        // Skip leading whitespace
        //
        while (p < s.size () && isspace (static_cast<unsigned char> (s [p])))
          ++p;

        if (p < s.size ())
          r.build_info = s.substr (p);
      }

      return r;
    }
  }
}
