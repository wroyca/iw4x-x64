// xxHash: convenience wrapper around xxHash64.
//
// This wrapper provides a simple, type-safe interface to xxHash64 for
// common hashing operations.
//
// Example usage:
//
//   // Hash a string.
//   uint64_t h1 (xxh64 ("hello world"));
//
//   // Hash with custom seed.
//   uint64_t h2 (xxh64 ("hello world", 42));
//
//   // Hash binary data.
//   const char data[] = {0x01, 0x02, 0x03};
//   uint64_t h3 (xxh64 (data, sizeof (data)));
//
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  namespace utility
  {
    // Compute xxHash64 of a string with default seed (0).
    //
    LIBIW4X_UTILITY_SYMEXPORT std::uint64_t
    xxh64 (const std::string& input);

    // Compute xxHash64 of a string with custom seed.
    //
    LIBIW4X_UTILITY_SYMEXPORT std::uint64_t
    xxh64 (const std::string& input, std::uint64_t seed);

    // Compute xxHash64 of binary data with default seed (0).
    //
    LIBIW4X_UTILITY_SYMEXPORT std::uint64_t
    xxh64 (const void* input, std::size_t length);

    // Compute xxHash64 of binary data with custom seed.
    //
    LIBIW4X_UTILITY_SYMEXPORT std::uint64_t
    xxh64 (const void* input, std::size_t length, std::uint64_t seed);

    // xxHash64 streaming hasher.
    //
    // Allows incremental hashing of data. This is useful when hashing large
    // amounts of data or when data arrives in chunks.
    //
    // Example usage:
    //
    //   xxh64_hasher hasher;
    //   hasher.update ("hello ");
    //   hasher.update ("world");
    //   uint64_t hash (hasher.digest ());
    //
    class LIBIW4X_UTILITY_SYMEXPORT xxh64_hasher
    {
    public:
      // Construct with default seed (0).
      //
      xxh64_hasher ();

      // Construct with custom seed.
      //
      explicit xxh64_hasher (std::uint64_t seed);

      ~xxh64_hasher ();

      xxh64_hasher (const xxh64_hasher&) = delete;
      xxh64_hasher& operator= (const xxh64_hasher&) = delete;

      // Reset the hasher state with default seed (0).
      //
      void
      reset ();

      // Reset the hasher state with custom seed.
      //
      void
      reset (std::uint64_t seed);

      // Add data to the hash.
      //
      void
      update (const std::string& input);

      // Add binary data to the hash.
      //
      void
      update (const void* input, std::size_t length);

      // Compute the final hash value.
      //
      // The hasher can continue to be used after calling digest().
      //
      std::uint64_t
      digest () const;

    private:
      struct state;
      state* state_;
    };
  }
}
