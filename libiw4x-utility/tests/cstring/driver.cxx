#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>

#include <libiw4x/utility/cstring.hxx>

#undef NDEBUG
#include <cassert>

int main ()
{
  using namespace std;
  namespace util = iw4x::utility;

  // Test memset with zero length (no-op)
  //
  {
    void* result (util::memset (nullptr, 0x42, 0));
    assert (result == nullptr);
  }

  // Test memset with nullptr and non-zero size
  //
  {
    try
    {
      util::memset (nullptr, 0x42, 10);
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (e.what () == string ("memset: null pointer with non-zero size"));
    }
  }

  // Test memset with regular value on heap memory
  //
  {
    vector<uint8_t> buffer (100, 0x00);
    void* result (util::memset (buffer.data (), 0x42, buffer.size ()));

    assert (result == buffer.data ());

    // Verify all bytes are set
    //
    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0x42);
  }

  // Test memset with different values
  //
  {
    vector<uint8_t> buffer (50);

    util::memset (buffer.data (), 0xFF, buffer.size ());
    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0xFF);

    util::memset (buffer.data (), 0x00, buffer.size ());
    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0x00);

    util::memset (buffer.data (), 0xAA, buffer.size ());
    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0xAA);
  }

  // Test memset with partial buffer
  //
  {
    vector<uint8_t> buffer (100, 0x00);

    util::memset (buffer.data (), 0x11, 25);

    // First 25 bytes should be 0x11
    //
    for (size_t i (0); i != 25; ++i)
      assert (buffer[i] == 0x11);

    // Rest should be 0x00
    //
    for (size_t i (25); i != buffer.size (); ++i)
      assert (buffer[i] == 0x00);
  }

  // Test memset with NOP sequences (0x90)
  //
  {
    vector<uint8_t> buffer (100, 0x00);

    util::memset (buffer.data (), 0x90, buffer.size ());

    // All bytes should contain valid NOP patterns
    // At minimum, the buffer should not be all zeros
    //
    bool has_nop (false);
    for (size_t i (0); i != buffer.size (); ++i)
    {
      if (buffer[i] == 0x90 || buffer[i] == 0x66 || buffer[i] == 0x0F)
      {
        has_nop = true;
        break;
      }
    }
    assert (has_nop);
  }

  // Test memset with NOP sequences - exact 9 bytes
  //
  {
    vector<uint8_t> buffer (9, 0x00);

    util::memset (buffer.data (), 0x90, 9);

    // Should contain 9-byte NOP sequence
    //
    assert (buffer[0] == 0x66);
    assert (buffer[1] == 0x0F);
    assert (buffer[2] == 0x1F);
    assert (buffer[3] == 0x84);
  }

  // Test memset with NOP sequences - 18 bytes (two 9-byte sequences)
  //
  {
    vector<uint8_t> buffer (18, 0x00);

    util::memset (buffer.data (), 0x90, 18);

    // First 9 bytes
    //
    assert (buffer[0] == 0x66);
    assert (buffer[1] == 0x0F);
    assert (buffer[2] == 0x1F);
    assert (buffer[3] == 0x84);

    // Second 9 bytes (should repeat)
    //
    assert (buffer[9] == 0x66);
    assert (buffer[10] == 0x0F);
    assert (buffer[11] == 0x1F);
    assert (buffer[12] == 0x84);
  }

  // Test memset with NOP sequences - 10 bytes (9 + 1)
  //
  {
    vector<uint8_t> buffer (10, 0x00);

    util::memset (buffer.data (), 0x90, 10);

    // First 9 bytes: 9-byte NOP sequence
    //
    assert (buffer[0] == 0x66);
    assert (buffer[1] == 0x0F);
    assert (buffer[2] == 0x1F);
    assert (buffer[3] == 0x84);

    // Last byte: 1-byte NOP (0x90)
    //
    assert (buffer[9] == 0x90);
  }

  // Test memset with NOP sequences - various tail sizes
  //
  {
    // 1-byte tail
    //
    {
      vector<uint8_t> buffer (1, 0x00);
      util::memset (buffer.data (), 0x90, 1);
      assert (buffer[0] == 0x90);
    }

    // 2-byte tail
    //
    {
      vector<uint8_t> buffer (2, 0x00);
      util::memset (buffer.data (), 0x90, 2);
      assert (buffer[0] == 0x66);
      assert (buffer[1] == 0x90);
    }

    // 3-byte tail
    //
    {
      vector<uint8_t> buffer (3, 0x00);
      util::memset (buffer.data (), 0x90, 3);
      assert (buffer[0] == 0x0F);
      assert (buffer[1] == 0x1F);
      assert (buffer[2] == 0x00);
    }
  }

  // Test memset with single byte
  //
  {
    vector<uint8_t> buffer (1, 0x00);

    util::memset (buffer.data (), 0x55, 1);

    assert (buffer[0] == 0x55);
  }

  // Test memset with large buffer
  //
  {
    vector<uint8_t> buffer (10000, 0x00);

    util::memset (buffer.data (), 0x77, buffer.size ());

    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0x77);
  }

  // Test memset uintptr_t overload with zero length
  //
  {
    uintptr_t addr (0);
    void* result (util::memset (addr, 0x42, 0));
    assert (result == nullptr);
  }

  // Test memset uintptr_t overload with valid address
  //
  {
    vector<uint8_t> buffer (100, 0x00);
    uintptr_t addr (reinterpret_cast<uintptr_t> (buffer.data ()));

    void* result (util::memset (addr, 0x33, buffer.size ()));

    assert (result == buffer.data ());

    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0x33);
  }

  // Test memset uintptr_t overload with NOP
  //
  {
    vector<uint8_t> buffer (20, 0x00);
    uintptr_t addr (reinterpret_cast<uintptr_t> (buffer.data ()));

    util::memset (addr, 0x90, buffer.size ());

    // Verify NOP patterns are present
    //
    bool has_nop (false);
    for (size_t i (0); i != buffer.size (); ++i)
    {
      if (buffer[i] == 0x90 || buffer[i] == 0x66 || buffer[i] == 0x0F)
      {
        has_nop = true;
        break;
      }
    }
    assert (has_nop);
  }

  // Test memset preserves adjacent memory
  //
  {
    vector<uint8_t> buffer (100, 0xCC);

    // Set middle 50 bytes
    //
    util::memset (buffer.data () + 25, 0x11, 50);

    // First 25 bytes should be untouched
    //
    for (size_t i (0); i != 25; ++i)
      assert (buffer[i] == 0xCC);

    // Middle 50 bytes should be 0x11
    //
    for (size_t i (25); i != 75; ++i)
      assert (buffer[i] == 0x11);

    // Last 25 bytes should be untouched
    //
    for (size_t i (75); i != buffer.size (); ++i)
      assert (buffer[i] == 0xCC);
  }

  // Test memset can be called multiple times on same memory
  //
  {
    vector<uint8_t> buffer (50, 0x00);

    util::memset (buffer.data (), 0x11, buffer.size ());
    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0x11);

    util::memset (buffer.data (), 0x22, buffer.size ());
    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0x22);

    util::memset (buffer.data (), 0x33, buffer.size ());
    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0x33);
  }

  // Test memset with overlapping regions
  //
  {
    vector<uint8_t> buffer (100, 0x00);

    util::memset (buffer.data (), 0xAA, 60);
    util::memset (buffer.data () + 40, 0xBB, 60);

    // First 40 bytes should be 0xAA
    //
    for (size_t i (0); i != 40; ++i)
      assert (buffer[i] == 0xAA);

    // Last 60 bytes should be 0xBB
    //
    for (size_t i (40); i != buffer.size (); ++i)
      assert (buffer[i] == 0xBB);
  }

  // Test memset return value
  //
  {
    vector<uint8_t> buffer (100);
    void* ptr (buffer.data ());

    void* result (util::memset (ptr, 0x99, buffer.size ()));

    assert (result == ptr);
  }

  // Test memset with maximum byte value
  //
  {
    vector<uint8_t> buffer (50);

    util::memset (buffer.data (), 0xFF, buffer.size ());

    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0xFF);
  }

  // Test memset with minimum byte value
  //
  {
    vector<uint8_t> buffer (50, 0xFF);

    util::memset (buffer.data (), 0x00, buffer.size ());

    for (size_t i (0); i != buffer.size (); ++i)
      assert (buffer[i] == 0x00);
  }

  // Test memset alignment doesn't matter
  //
  {
    vector<uint8_t> buffer (100, 0x00);

    // Unaligned address
    //
    void* unaligned (buffer.data () + 1);
    util::memset (unaligned, 0x44, 50);

    assert (buffer[0] == 0x00);
    for (size_t i (1); i != 51; ++i)
      assert (buffer[i] == 0x44);
    for (size_t i (51); i != buffer.size (); ++i)
      assert (buffer[i] == 0x00);
  }

  // Test memset with NOP on odd sizes
  //
  {
    for (size_t sz (1); sz != 20; ++sz)
    {
      vector<uint8_t> buffer (sz, 0x00);
      util::memset (buffer.data (), 0x90, sz);

      // Verify buffer is not all zeros (has been modified)
      //
      bool modified (false);
      for (size_t i (0); i != sz; ++i)
      {
        if (buffer[i] != 0x00)
        {
          modified = true;
          break;
        }
      }
      assert (modified);
    }
  }
}
