#include <libiw4x/utility/cstring.hxx>

#include <cstring>
#include <stdexcept>

#include <libiw4x/utility/utility-win32.hxx>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    void*
    memset (void* destination, int value, size_t size)
    {
      // Zero-length operation is no-op by definition (includes nullptr case).
      //
      if (size == 0)
        return destination;

      if (destination == nullptr)
        throw invalid_argument ("memset: null pointer with non-zero size");

      DWORD o;
      if (!VirtualProtect (destination, size, PAGE_EXECUTE_READWRITE, &o))
        throw runtime_error (format_message (GetLastError ()));

      auto m (std::memset (destination, value, size));

      if (!VirtualProtect (destination, size, o, &o))
        throw runtime_error (format_message (GetLastError ()));

      return m;
    }
  }
}
