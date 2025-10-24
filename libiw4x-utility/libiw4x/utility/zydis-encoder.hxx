#pragma once

#include <span>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

#include <immer/flex_vector.hpp>
#include <immer/flex_vector_transient.hpp>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  namespace utility
  {
    // Exception thrown when instruction encoding fails.
    //
    class LIBIW4X_UTILITY_SYMEXPORT encoding_error : public std::runtime_error
    {
    public:
      explicit encoding_error (const std::string& what)
        : std::runtime_error (what) {}
    };

    // Encoded instruction sequence represented as a persistent vector.
    //
    using instruction = immer::flex_vector<uint8_t>;

    // Instruction encoder.
    //
    class LIBIW4X_UTILITY_SYMEXPORT encoder
    {
    public:
      encoder () = default;

      // Control Transfer Instructions
      //
      encoder
      jmp (uint64_t absolute);

      // Append one or more encoded instructions to the current instruction
      // stream.
      //
      encoder
      append (std::span<const uint8_t> instruction);

      // Emit the completed instruction sequence.
      //
      instruction
      emit () const noexcept
      {
        return stream;
      }

      // Get the current size of the encoded instruction sequence.
      //
      size_t
      size () const noexcept
      {
        return stream.size ();
      }

      // Check if encoder is empty.
      //
      bool
      empty () const noexcept
      {
        return stream.empty ();
      }

    private:
      instruction stream;

      // Helper to encode an instruction sequence using Zydis.
      //
      encoder
      encode (auto&& request);
    };
  }
}
