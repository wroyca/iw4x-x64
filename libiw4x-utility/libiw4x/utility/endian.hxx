#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  // Detect host endianness.
  //
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  constexpr bool host_little_endian = true;
#else
  constexpr bool host_little_endian = false;
#endif

  // Byte swap.
  //

  inline uint8_t
  bswap (uint8_t v)
  {
    return v;
  }

  inline uint16_t
  bswap (uint16_t v)
  {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16 (v);
#else
    return _byteswap_ushort (v);
#endif
  }

  inline uint32_t
  bswap (uint32_t v)
  {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32 (v);
#else
    return _byteswap_ulong (v);
#endif
  }

  inline uint64_t
  bswap (uint64_t v)
  {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64 (v);
#else
    return _byteswap_uint64 (v);
#endif
  }

  // Convert between host and little endian.
  //

  inline uint8_t
  htole (uint8_t v)
  {
    return v;
  }

  inline uint16_t
  htole (uint16_t v)
  {
    return host_little_endian ? v : bswap (v);
  }

  inline uint32_t
  htole (uint32_t v)
  {
    return host_little_endian ? v : bswap (v);
  }

  inline uint64_t
  htole (uint64_t v)
  {
    return host_little_endian ? v : bswap (v);
  }

  inline uint8_t
  letoh (uint8_t v)
  {
    return v;
  }

  inline uint16_t
  letoh (uint16_t v)
  {
    return host_little_endian ? v : bswap (v);
  }

  inline uint32_t
  letoh (uint32_t v)
  {
    return host_little_endian ? v : bswap (v);
  }

  inline uint64_t
  letoh (uint64_t v)
  {
    return host_little_endian ? v : bswap (v);
  }

  // Convert between host and big endian.
  //

  inline uint8_t
  htobe (uint8_t v)
  {
    return v;
  }

  inline uint16_t
  htobe (uint16_t v)
  {
    return host_little_endian ? bswap (v) : v;
  }

  inline uint32_t
  htobe (uint32_t v)
  {
    return host_little_endian ? bswap (v) : v;
  }

  inline uint64_t
  htobe (uint64_t v)
  {
    return host_little_endian ? bswap (v) : v;
  }

  inline uint8_t
  betoh (uint8_t v)
  {
    return v;
  }

  inline uint16_t
  betoh (uint16_t v)
  {
    return host_little_endian ? bswap (v) : v;
  }

  inline uint32_t
  betoh (uint32_t v)
  {
    return host_little_endian ? bswap (v) : v;
  }

  inline uint64_t
  betoh (uint64_t v)
  {
    return host_little_endian ? bswap (v) : v;
  }

  // Store a value in a specific byte order.
  //

  inline void
  store_le (uint8_t* p, uint8_t v)
  {
    *p = v;
  }

  inline void
  store_le (uint8_t* p, uint16_t v)
  {
    v = htole (v);
    std::memcpy (p, &v, sizeof (v));
  }

  inline void
  store_le (uint8_t* p, uint32_t v)
  {
    v = htole (v);
    std::memcpy (p, &v, sizeof (v));
  }

  inline void
  store_le (uint8_t* p, uint64_t v)
  {
    v = htole (v);
    std::memcpy (p, &v, sizeof (v));
  }

  inline void
  store_be (uint8_t* p, uint8_t v)
  {
    *p = v;
  }

  inline void
  store_be (uint8_t* p, uint16_t v)
  {
    v = htobe (v);
    std::memcpy (p, &v, sizeof (v));
  }

  inline void
  store_be (uint8_t* p, uint32_t v)
  {
    v = htobe (v);
    std::memcpy (p, &v, sizeof (v));
  }

  inline void
  store_be (uint8_t* p, uint64_t v)
  {
    v = htobe (v);
    std::memcpy (p, &v, sizeof (v));
  }

  // Load a value in a specific byte order.
  //

  inline uint8_t
  load_le8 (const uint8_t* p)
  {
    return *p;
  }

  inline uint16_t
  load_le16 (const uint8_t* p)
  {
    uint16_t v;
    std::memcpy (&v, p, sizeof (v));
    return letoh (v);
  }

  inline uint32_t
  load_le32 (const uint8_t* p)
  {
    uint32_t v;
    std::memcpy (&v, p, sizeof (v));
    return letoh (v);
  }

  inline uint64_t
  load_le64 (const uint8_t* p)
  {
    uint64_t v;
    std::memcpy (&v, p, sizeof (v));
    return letoh (v);
  }

  inline uint8_t
  load_be8 (const uint8_t* p)
  {
    return *p;
  }

  inline uint16_t
  load_be16 (const uint8_t* p)
  {
    uint16_t v;
    std::memcpy (&v, p, sizeof (v));
    return betoh (v);
  }

  inline uint32_t
  load_be32 (const uint8_t* p)
  {
    uint32_t v;
    std::memcpy (&v, p, sizeof (v));
    return betoh (v);
  }

  inline uint64_t
  load_be64 (const uint8_t* p)
  {
    uint64_t v;
    std::memcpy (&v, p, sizeof (v));
    return betoh (v);
  }
}
