#include <libiw4x/utility/cstring.hxx>

#include <array>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <libiw4x/utility/utility-win32.hxx>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    static constexpr array<array<uint8_t, 9>, 9> nop_sequences
    {{
      {{0x90}},
      {{0x66, 0x90}},
      {{0x0F, 0x1F, 0x00}},
      {{0x0F, 0x1F, 0x40, 0x00}},
      {{0x0F, 0x1F, 0x44, 0x00, 0x00}},
      {{0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00}},
      {{0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00}},
      {{0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00}},
      {{0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00}}
    }};

    void*
    memset (void* dst, int v, size_t n)
    {
      // Zero-length operation is no-op by definition (includes nullptr case).
      //
      if (n == 0)
        return dst;

      if (dst == nullptr)
        throw invalid_argument ("memset: null pointer with non-zero size");

      // Switch region to RWX. Note that VirtualProtect() works in page
      // granularity and will widen the range as it pleases.
      //
      DWORD o;
      if (!VirtualProtect (dst, n, PAGE_EXECUTE_READWRITE, &o))
        throw runtime_error (format_message (GetLastError ()));

      void* r (nullptr);

      // Treat 0x90 as a request for architectural NOP sequences.
      //
      if (v == 0x90)
      {
        uint8_t* d (reinterpret_cast<uint8_t*> (dst));

        // Use 9-byte sequence as the bulk filler.
        //
        const uint8_t* s (nop_sequences[8].data ());

        size_t q (n / 9);
        for (size_t i (0); i != q; ++i)
        {
          memcpy (d, s, 9);
          d += 9;
        }

        // Any tail < 9 bytes is satisfied by the corresponding canonical form.
        //
        size_t rem (n % 9);
        if (rem != 0)
          memcpy (d, nop_sequences[rem - 1].data (), rem);

        r = dst;
      }
      else
      {
        // For all non-NOP cases, fall back to the ordinary byte blast.
        //
        r = std::memset (dst, v, n);
      }

      // Restore protection. If the system decides that the restored permissions
      // are not what they used to be, then that is its prerogative, and our cue
      // to bail.
      //
      if (!VirtualProtect (dst, n, o, &o))
        throw runtime_error (format_message (GetLastError ()));

      return r;
    }
  }
}
