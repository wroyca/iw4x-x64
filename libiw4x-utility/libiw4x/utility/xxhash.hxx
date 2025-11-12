#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
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
    xxh64 (std::span<const std::byte> input);

    // Compute xxHash64 of binary data with custom seed.
    //
    LIBIW4X_UTILITY_SYMEXPORT std::uint64_t
    xxh64 (std::span<const std::byte> input, std::uint64_t seed);

    // xxHash64 streaming hasher.
    //
    class LIBIW4X_UTILITY_SYMEXPORT xxh64_hasher
    {
    public:
      // Construct with default seed (0).
      //
      xxh64_hasher ();

      // Construct with custom seed.
      //
      explicit
      xxh64_hasher (std::uint64_t seed);

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
      update (std::span<const std::byte> input);

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
