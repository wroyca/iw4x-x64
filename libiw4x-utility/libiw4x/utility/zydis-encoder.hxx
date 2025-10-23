#pragma once

#include <cstdint>
#include <iosfwd>

#include <immer/vector.hpp>
#include <immer/vector_transient.hpp>

#include <libiw4x/utility/export.hxx>

// Forward declarations to avoid pulling in full Zydis headers.
//
// TODO: apply the same approach to immer
//
struct ZydisEncoderRequest_;
using ZydisEncoderRequest = ZydisEncoderRequest_;

namespace iw4x
{
  namespace utility
  {
    class LIBIW4X_UTILITY_SYMEXPORT encoder
    {
    public:
      using bytes = immer::vector<std::uint8_t>;

      encoder () = default;

      // Absolute jump (rip-relative memory indirect).
      //
      template <typename Self>
      [[nodiscard]] encoder
      jmp (this Self&& self, std::uint64_t target);

      // Relative jump (signed 32-bit offset).
      //
      template <typename Self>
      [[nodiscard]] encoder
      jmp_rel (this Self&& self, std::int32_t offset);

      // Conditional jump (relative, signed 32-bit offset).
      //
      enum class condition
      {
        e,   // equal / zero
        ne,  // not equal / not zero
        l,   // less (signed)
        le,  // less or equal (signed)
        g,   // greater (signed)
        ge,  // greater or equal (signed)
        b,   // below (unsigned)
        be,  // below or equal (unsigned)
        a,   // above (unsigned)
        ae   // above or equal (unsigned)
      };

      template <typename Self>
      [[nodiscard]] encoder
      jcc (this Self&& self, condition cond, std::int32_t offset);

      // Call instruction (absolute address via rip-relative).
      //
      template <typename Self>
      [[nodiscard]] encoder
      call (this Self&& self, std::uint64_t target);

      // Call instruction (relative offset).
      //
      template <typename Self>
      [[nodiscard]] encoder
      call_rel (this Self&& self, std::int32_t offset);

      // Push register.
      //
      enum class reg64
      {
        rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi,
        r8, r9, r10, r11, r12, r13, r14, r15
      };

      template <typename Self>
      [[nodiscard]] encoder
      push (this Self&& self, reg64 r);

      // Pop register.
      //
      template <typename Self>
      [[nodiscard]] encoder
      pop (this Self&& self, reg64 r);

      // Move immediate to register.
      //
      template <typename Self>
      [[nodiscard]] encoder
      mov (this Self&& self, reg64 dst, std::uint64_t imm);

      // Single-byte NOP.
      //
      template <typename Self>
      [[nodiscard]] encoder
      nop (this Self&& self);

      // Multi-byte NOP (1-9 bytes, Intel-recommended sequences).
      //
      template <typename Self>
      [[nodiscard]] encoder
      nop (this Self&& self, std::size_t n);

      // Near return.
      //
      template <typename Self>
      [[nodiscard]] encoder
      ret (this Self&& self);

      // Return with immediate (pop n bytes).
      //
      template <typename Self>
      [[nodiscard]] encoder
      ret (this Self&& self, std::uint16_t n);

      // Interrupt.
      //
      template <typename Self>
      [[nodiscard]] encoder
      int3 (this Self&& self);

      // Emit all instructions as flat byte sequence.
      //
      [[nodiscard]] bytes
      emit () const noexcept;

      // Get total byte size.
      //
      [[nodiscard]] std::size_t
      size () const noexcept;

      // Check if encoder is empty.
      //
      [[nodiscard]] bool
      empty () const noexcept;

      // Stream output (hex dump).
      //
      friend LIBIW4X_UTILITY_SYMEXPORT std::ostream&
      operator<< (std::ostream&, const encoder&);

    private:
      bytes code_;

      // Encode single instruction.
      //
      template <typename Self, typename F>
      [[nodiscard]] encoder
      encode (this Self&& self, F&& setup);

      // Append instruction to encoder sequence.
      //
      template <typename Self, typename... Ts>
      [[nodiscard]] encoder
      append (this Self&& self, Ts... bs);
    };

    // Write encoded instructions to memory.
    //
    LIBIW4X_UTILITY_SYMEXPORT void
    write (std::uint64_t address, const encoder::bytes& code);

    // Convenience.
    //
    LIBIW4X_UTILITY_SYMEXPORT void
    patch_jmp (std::uint64_t address, std::uint64_t target);

    LIBIW4X_UTILITY_SYMEXPORT void
    patch_call (std::uint64_t address, std::uint64_t target);

    LIBIW4X_UTILITY_SYMEXPORT void
    patch_nop (std::uint64_t address, std::size_t n);

    LIBIW4X_UTILITY_SYMEXPORT void
    patch_ret (std::uint64_t address);
  }
}
