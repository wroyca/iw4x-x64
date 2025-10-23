#include <libiw4x/utility/zydis-encoder.hxx>

#include <cstring>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <array>

#include <Zydis/Zydis.h>
#include <boost/endian/conversion.hpp>

#include <libiw4x/utility/utility-win32.hxx>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    namespace
    {
      // Intel-recommended multi-byte NOP sequences.
      //
      // Note: Only 1-9 byte sequences are officially documented by Intel.
      //
      constexpr array<array<uint8_t, 9>, 9> nop_sequences
      {{
        {0x90},
        {0x66, 0x90},
        {0x0F, 0x1F, 0x00},
        {0x0F, 0x1F, 0x40, 0x00},
        {0x0F, 0x1F, 0x44, 0x00, 0x00},
        {0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00},
        {0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00},
        {0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00}
      }};

      // Map condition enum to Zydis mnemonic.
      //
      // Note: Some condition codes are aliases in assembly but Zydis uses
      // canonical mnemonic names:
      //
      //   JG  -> JNLE (jump if not less or equal, signed)
      //   JGE -> JNL  (jump if not less, signed)
      //   JA  -> JNBE (jump if not below or equal, unsigned)
      //   JAE -> JNB  (jump if not below, unsigned)
      //
      constexpr ZydisMnemonic
      condition_to_mnemonic (encoder::condition cond) noexcept
      {
        switch (cond)
        {
          case encoder::condition::e:  return ZYDIS_MNEMONIC_JZ;
          case encoder::condition::ne: return ZYDIS_MNEMONIC_JNZ;
          case encoder::condition::l:  return ZYDIS_MNEMONIC_JL;
          case encoder::condition::le: return ZYDIS_MNEMONIC_JLE;
          case encoder::condition::g:  return ZYDIS_MNEMONIC_JNLE;  // Alias: JG
          case encoder::condition::ge: return ZYDIS_MNEMONIC_JNL;   // Alias: JGE
          case encoder::condition::b:  return ZYDIS_MNEMONIC_JB;
          case encoder::condition::be: return ZYDIS_MNEMONIC_JBE;
          case encoder::condition::a:  return ZYDIS_MNEMONIC_JNBE;  // Alias: JA
          case encoder::condition::ae: return ZYDIS_MNEMONIC_JNB;   // Alias: JAE
          default: return ZYDIS_MNEMONIC_INVALID;
        }
      }

      // Map reg64 enum to Zydis register.
      //
      constexpr ZydisRegister
      reg64_to_zydis (encoder::reg64 r) noexcept
      {
        switch (r)
        {
          case encoder::reg64::rax: return ZYDIS_REGISTER_RAX;
          case encoder::reg64::rcx: return ZYDIS_REGISTER_RCX;
          case encoder::reg64::rdx: return ZYDIS_REGISTER_RDX;
          case encoder::reg64::rbx: return ZYDIS_REGISTER_RBX;
          case encoder::reg64::rsp: return ZYDIS_REGISTER_RSP;
          case encoder::reg64::rbp: return ZYDIS_REGISTER_RBP;
          case encoder::reg64::rsi: return ZYDIS_REGISTER_RSI;
          case encoder::reg64::rdi: return ZYDIS_REGISTER_RDI;
          case encoder::reg64::r8:  return ZYDIS_REGISTER_R8;
          case encoder::reg64::r9:  return ZYDIS_REGISTER_R9;
          case encoder::reg64::r10: return ZYDIS_REGISTER_R10;
          case encoder::reg64::r11: return ZYDIS_REGISTER_R11;
          case encoder::reg64::r12: return ZYDIS_REGISTER_R12;
          case encoder::reg64::r13: return ZYDIS_REGISTER_R13;
          case encoder::reg64::r14: return ZYDIS_REGISTER_R14;
          case encoder::reg64::r15: return ZYDIS_REGISTER_R15;
          default: return ZYDIS_REGISTER_NONE;
        }
      }
    }

    // Encoder
    //

    template <typename Self, typename F>
    encoder encoder::
    encode (this Self&& self, F&& setup)
    {
      ZydisEncoderRequest req;
      memset (&req, 0, sizeof (req));
      req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;

      // Let caller configure the request.
      //
      setup (req);

      // Encode instruction.
      //
      std::array<uint8_t, ZYDIS_MAX_INSTRUCTION_LENGTH> buffer;
      size_t length (buffer.size ());

      if (ZYAN_FAILED (ZydisEncoderEncodeInstruction (&req, buffer.data (), &length)))
        throw runtime_error ("Zydis encoding failed");

      // Append to code using transient as transactional.
      //
      auto t (std::forward<Self> (self).code_.transient ());
      for (size_t i (0); i < length; ++i)
        t.push_back (buffer[i]);

      encoder r;
      r.code_ = t.persistent ();
      return r;
    }

    template <typename Self, typename... Ts>
    encoder encoder::
    append (this Self&& self, Ts... bs)
    {
      auto t (std::forward<Self> (self).code_.transient ());
      (t.push_back (bs), ...);

      encoder r;
      r.code_ = t.persistent ();
      return r;
    }

    template <typename Self>
    encoder encoder::
    jmp (this Self&& self, uint64_t target)
    {
      auto enc (std::forward<Self> (self).encode ([](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_JMP;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_MEMORY;
        req.operands[0].mem.base = ZYDIS_REGISTER_RIP;
        req.operands[0].mem.displacement = 0;
        req.operands[0].mem.size = 8;
      }));

      uint64_t target_le (boost::endian::native_to_little (target));
      uint8_t* bytes (reinterpret_cast<uint8_t*> (&target_le));

      return move (enc).append (
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5], bytes[6], bytes[7]
      );
    }

    template <typename Self>
    encoder encoder::
    jmp_rel (this Self&& self, int32_t offset)
    {
      return std::forward<Self> (self).encode ([offset](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_JMP;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[0].imm.s = offset;
      });
    }

    template <typename Self>
    encoder encoder::
    jcc (this Self&& self, condition cond, int32_t offset)
    {
      return std::forward<Self> (self).encode ([cond, offset](ZydisEncoderRequest& req)
      {
        req.mnemonic = condition_to_mnemonic (cond);
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[0].imm.s = offset;
      });
    }

    template <typename Self>
    encoder encoder::
    call (this Self&& self, uint64_t target)
    {
      auto enc (std::forward<Self> (self).encode ([](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_CALL;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_MEMORY;
        req.operands[0].mem.base = ZYDIS_REGISTER_RIP;
        req.operands[0].mem.displacement = 0;
        req.operands[0].mem.size = 8;
      }));

      uint64_t target_le (boost::endian::native_to_little (target));
      uint8_t* bytes (reinterpret_cast<uint8_t*> (&target_le));

      return move (enc).append (
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5], bytes[6], bytes[7]
      );
    }

    template <typename Self>
    encoder encoder::
    call_rel (this Self&& self, int32_t offset)
    {
      return std::forward<Self> (self).encode ([offset](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_CALL;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[0].imm.s = offset;
      });
    }

    template <typename Self>
    encoder encoder::
    push (this Self&& self, reg64 r)
    {
      return std::forward<Self> (self).encode ([r](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_PUSH;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
        req.operands[0].reg.value = reg64_to_zydis (r);
      });
    }

    template <typename Self>
    encoder encoder::
    pop (this Self&& self, reg64 r)
    {
      return std::forward<Self> (self).encode ([r](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_POP;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
        req.operands[0].reg.value = reg64_to_zydis (r);
      });
    }

    template <typename Self>
    encoder encoder::
    mov (this Self&& self, reg64 dst, uint64_t imm)
    {
      return std::forward<Self> (self).encode ([dst, imm](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_MOV;
        req.operand_count = 2;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
        req.operands[0].reg.value = reg64_to_zydis (dst);
        req.operands[1].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[1].imm.u = imm;
      });
    }

    template <typename Self>
    encoder encoder::
    nop (this Self&& self)
    {
      return std::forward<Self> (self).append (0x90);
    }

    template <typename Self>
    encoder encoder::
    nop (this Self&& self, size_t n)
    {
      if (n == 0)
        return std::forward<Self> (self);

      if (n > 9)
        throw runtime_error ("NOP size exceeds maximum (9 bytes)");

      auto t (std::forward<Self> (self).code_.transient ());
      const auto& seq (nop_sequences[n - 1]);

      for (size_t i (0); i < n; ++i)
        t.push_back (seq[i]);

      encoder r;
      r.code_ = t.persistent ();
      return r;
    }

    template <typename Self>
    encoder encoder::
    ret (this Self&& self)
    {
      return std::forward<Self> (self).append (0xC3);
    }

    template <typename Self>
    encoder encoder::
    ret (this Self&& self, uint16_t n)
    {
      return std::forward<Self> (self).encode ([n](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_RET;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[0].imm.u = n;
      });
    }

    template <typename Self>
    encoder encoder::
    int3 (this Self&& self)
    {
      return std::forward<Self> (self).append (0xCC);
    }

    encoder::bytes encoder::
    emit () const noexcept
    {
      return code_;
    }

    size_t encoder::
    size () const noexcept
    {
      return code_.size ();
    }

    bool encoder::
    empty () const noexcept
    {
      return code_.empty ();
    }

    ostream&
    operator<< (ostream& os, const encoder& enc)
    {
      const auto& code (enc.code_);

      if (code.empty ())
      {
        os << "(empty)";
        return os;
      }

      ios_base::fmtflags flags (os.flags ());
      char fill (os.fill ());

      os << hex << setfill ('0');

      for (size_t i (0); i < code.size (); ++i)
      {
        if (i > 0)
          os << ' ';
        os << setw (2) << static_cast<unsigned> (code[i]);
      }

      os.flags (flags);
      os.fill (fill);

      return os;
    }

    // Memory patching.
    //

    void
    write (uint64_t addr, const encoder::bytes& code)
    {
      if (code.empty ())
        throw runtime_error ("cannot write empty code sequence");

      size_t len (code.size ());

      DWORD old_protection (0);

      if (VirtualProtect (reinterpret_cast<LPVOID> (addr),
                          code.size (),
                          PAGE_READWRITE,
                          &old_protection) == 0)
      {
        throw runtime_error ("unable to change page protection");
      }

      uint8_t* dest (reinterpret_cast<uint8_t*> (addr));
      for (size_t i (0); i < len; ++i)
        dest[i] = code[i];

      if (VirtualProtect (reinterpret_cast<LPVOID> (addr),
                          code.size (),
                          old_protection,
                          &old_protection) == 0)
      {
        throw runtime_error ("unable to restore page protection");
      }

      if (FlushInstructionCache (GetCurrentProcess (),
                                 reinterpret_cast<LPCVOID> (addr),
                                 code.size ()) == 0)
      {
        throw runtime_error ("unable to flush instruction cache");
      }
    }

    // Convenience.
    //

    void
    patch_jmp (uint64_t addr, uint64_t target)
    {
      write (addr, encoder ().jmp (target).emit ());
    }

    void
    patch_call (uint64_t addr, uint64_t target)
    {
      write (addr, encoder ().call (target).emit ());
    }

    void
    patch_nop (uint64_t addr, size_t n)
    {
      write (addr, encoder ().nop (n).emit ());
    }

    void
    patch_ret (uint64_t addr)
    {
      write (addr, encoder ().ret ().emit ());
    }
  }
}
