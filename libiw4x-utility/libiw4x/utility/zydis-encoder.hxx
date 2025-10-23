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
      [[nodiscard]] encoder
      jmp (this auto&& self, std::uint64_t target);

      // Relative jump (signed 32-bit offset).
      //
      [[nodiscard]] encoder
      jmp_rel (this auto&& self, std::int32_t offset);

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

      [[nodiscard]] encoder
      jcc (this auto&& self, condition cond, std::int32_t offset);

      // Call instruction (absolute address via rip-relative).
      //
      [[nodiscard]] encoder
      call (this auto&& self, std::uint64_t target);

      // Call instruction (relative offset).
      //
      [[nodiscard]] encoder
      call_rel (this auto&& self, std::int32_t offset);

      // Push register.
      //
      enum class reg64
      {
        rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi,
        r8, r9, r10, r11, r12, r13, r14, r15
      };

      [[nodiscard]] encoder
      push (this auto&& self, reg64 r);

      // Pop register.
      //
      [[nodiscard]] encoder
      pop (this auto&& self, reg64 r);

      // Move immediate to register.
      //
      [[nodiscard]] encoder
      mov (this auto&& self, reg64 dst, std::uint64_t imm);

      // Single-byte NOP.
      //
      [[nodiscard]] encoder
      nop (this auto&& self);

      // Multi-byte NOP (1-9 bytes, Intel-recommended sequences).
      //
      [[nodiscard]] encoder
      nop (this auto&& self, std::size_t n);

      // Near return.
      //
      [[nodiscard]] encoder
      ret (this auto&& self);

      // Return with immediate (pop n bytes).
      //
      [[nodiscard]] encoder
      ret (this auto&& self, std::uint16_t n);

      // Interrupt.
      //
      [[nodiscard]] encoder
      int3 (this auto&& self);

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
      [[nodiscard]] encoder
      encode (this auto&& self, auto&& callback);

      // Append instruction to encoder sequence.
      //
      [[nodiscard]] encoder
      append (this auto&& self, auto... instruction);
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
