#include <iostream>
#include <cstdint>
#include <cstring>
#include <cassert>

#include <libiw4x/utility/endian.hxx>

#undef NDEBUG
#include <cassert>

int
main ()
{
  using namespace iw4x;

  // Test byte swap functions
  //
  assert (bswap (uint8_t (0x12)) == 0x12);
  assert (bswap (uint16_t (0x1234)) == 0x3412);
  assert (bswap (uint32_t (0x12345678)) == 0x78563412);
  assert (bswap (uint64_t (0x123456789abcdef0)) == 0xf0debc9a78563412);

  // Test host to little endian conversion
  //
  assert (htole (uint8_t (0x12)) == 0x12);

  if (host_little_endian)
  {
    assert (htole (uint16_t (0x1234)) == 0x1234);
    assert (htole (uint32_t (0x12345678)) == 0x12345678);
    assert (htole (uint64_t (0x123456789abcdef0)) == 0x123456789abcdef0);
  }
  else
  {
    assert (htole (uint16_t (0x1234)) == 0x3412);
    assert (htole (uint32_t (0x12345678)) == 0x78563412);
    assert (htole (uint64_t (0x123456789abcdef0)) == 0xf0debc9a78563412);
  }

  // Test little endian to host conversion
  //
  assert (letoh (uint8_t (0x12)) == 0x12);

  if (host_little_endian)
  {
    assert (letoh (uint16_t (0x1234)) == 0x1234);
    assert (letoh (uint32_t (0x12345678)) == 0x12345678);
    assert (letoh (uint64_t (0x123456789abcdef0)) == 0x123456789abcdef0);
  }
  else
  {
    assert (letoh (uint16_t (0x1234)) == 0x3412);
    assert (letoh (uint32_t (0x12345678)) == 0x78563412);
    assert (letoh (uint64_t (0x123456789abcdef0)) == 0xf0debc9a78563412);
  }

  // Test host to big endian conversion
  //
  assert (htobe (uint8_t (0x12)) == 0x12);

  if (host_little_endian)
  {
    assert (htobe (uint16_t (0x1234)) == 0x3412);
    assert (htobe (uint32_t (0x12345678)) == 0x78563412);
    assert (htobe (uint64_t (0x123456789abcdef0)) == 0xf0debc9a78563412);
  }
  else
  {
    assert (htobe (uint16_t (0x1234)) == 0x1234);
    assert (htobe (uint32_t (0x12345678)) == 0x12345678);
    assert (htobe (uint64_t (0x123456789abcdef0)) == 0x123456789abcdef0);
  }

  // Test big endian to host conversion
  //
  assert (betoh (uint8_t (0x12)) == 0x12);

  if (host_little_endian)
  {
    assert (betoh (uint16_t (0x1234)) == 0x3412);
    assert (betoh (uint32_t (0x12345678)) == 0x78563412);
    assert (betoh (uint64_t (0x123456789abcdef0)) == 0xf0debc9a78563412);
  }
  else
  {
    assert (betoh (uint16_t (0x1234)) == 0x1234);
    assert (betoh (uint32_t (0x12345678)) == 0x12345678);
    assert (betoh (uint64_t (0x123456789abcdef0)) == 0x123456789abcdef0);
  }

  // Test store little endian
  //
  {
    uint8_t buf[2];
    store_le (buf, uint16_t (0x1234));
    assert (buf[0] == 0x34 && buf[1] == 0x12);
  }

  {
    uint8_t buf[4];
    store_le (buf, uint32_t (0x12345678));
    assert (buf[0] == 0x78 && buf[1] == 0x56 && buf[2] == 0x34 && buf[3] == 0x12);
  }

  {
    uint8_t buf[8];
    store_le (buf, uint64_t (0x123456789abcdef0));
    assert (buf[0] == 0xf0 && buf[1] == 0xde && buf[2] == 0xbc && buf[3] == 0x9a &&
            buf[4] == 0x78 && buf[5] == 0x56 && buf[6] == 0x34 && buf[7] == 0x12);
  }

  // Test store big endian
  //
  {
    uint8_t buf[2];
    store_be (buf, uint16_t (0x1234));
    assert (buf[0] == 0x12 && buf[1] == 0x34);
  }

  {
    uint8_t buf[4];
    store_be (buf, uint32_t (0x12345678));
    assert (buf[0] == 0x12 && buf[1] == 0x34 && buf[2] == 0x56 && buf[3] == 0x78);
  }

  {
    uint8_t buf[8];
    store_be (buf, uint64_t (0x123456789abcdef0));
    assert (buf[0] == 0x12 && buf[1] == 0x34 && buf[2] == 0x56 && buf[3] == 0x78 &&
            buf[4] == 0x9a && buf[5] == 0xbc && buf[6] == 0xde && buf[7] == 0xf0);
  }

  // Test load little endian
  //
  {
    uint8_t buf[] (0x34, 0x12);
    assert (load_le16 (buf) == 0x1234);
  }

  {
    uint8_t buf[] (0x78, 0x56, 0x34, 0x12);
    assert (load_le32 (buf) == 0x12345678);
  }

  {
    uint8_t buf[] (0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12);
    assert (load_le64 (buf) == 0x123456789abcdef0);
  }

  // Test load big endian
  //
  {
    uint8_t buf[] (0x12, 0x34);
    assert (load_be16 (buf) == 0x1234);
  }

  {
    uint8_t buf[] (0x12, 0x34, 0x56, 0x78);
    assert (load_be32 (buf) == 0x12345678);
  }

  {
    uint8_t buf[] (0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0);
    assert (load_be64 (buf) == 0x123456789abcdef0);
  }

  // Test round-trip
  //
  {
    uint64_t orig (0x123456789abcdef0);
    uint8_t buf[8];

    // Little endian round-trip
    //
    store_le (buf, orig);
    uint64_t result (load_le64 (buf));
    assert (result == orig);

    // Big endian round-trip
    //
    store_be (buf, orig);
    result = load_be64 (buf);
    assert (result == orig);
  }
}
