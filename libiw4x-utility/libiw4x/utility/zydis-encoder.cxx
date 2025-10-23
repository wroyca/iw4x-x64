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
          case encoder::condition::g:  return ZYDIS_MNEMONIC_JNLE;
          case encoder::condition::ge: return ZYDIS_MNEMONIC_JNL;
          case encoder::condition::b:  return ZYDIS_MNEMONIC_JB;
          case encoder::condition::be: return ZYDIS_MNEMONIC_JBE;
          case encoder::condition::a:  return ZYDIS_MNEMONIC_JNBE;
          case encoder::condition::ae: return ZYDIS_MNEMONIC_JNB;
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

    encoder
    encoder::encode (this auto&& s, auto&& c)
    {
      ZydisEncoderRequest r {};
      r.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;

      // Let caller configure the request.
      //
      c (r);

      // Encode instruction.
      //
      ZyanU8 b [ZYDIS_MAX_INSTRUCTION_LENGTH];
      ZyanUSize l (sizeof (b));

      if (ZYAN_FAILED (ZydisEncoderEncodeInstruction (&r, b, &l)))
        throw runtime_error ("unable to encode instruction");

      // Append encoded bytes to code buffer.
      //
      auto t (std::forward<decltype(s)> (s).code_.transient ());
      for (size_t i (0); i < l; ++i)
        t.push_back (b[i]);

      encoder result;
      result.code_ = t.persistent ();
      return result;
    }

    encoder
    encoder::append (this auto&& s, auto... instruction)
    {
      auto t (forward<decltype (s)> (s).code_.transient ());
      {
        (
          t.push_back (instruction),
          ...
        );
      }

      encoder r;
      r.code_ = t.persistent ();
      return r;
    }

    encoder
    encoder::jmp (this auto&& s, uint64_t t)
    {
      auto e (forward<decltype(s)> (s).encode ([](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_JMP;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_MEMORY;
        req.operands[0].mem.base = ZYDIS_REGISTER_RIP;
        req.operands[0].mem.displacement = 0;
        req.operands[0].mem.size = 8;
      }));

      auto l (boost::endian::native_to_little (t));
      auto b (reinterpret_cast<uint8_t*> (&l));

      return move (e).append (b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
    }

    encoder encoder::
    jmp_rel (this auto&& s, int32_t o)
    {
      return forward<decltype(s)> (s).encode ([o](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_JMP;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[0].imm.s = o;
      });
    }

    encoder encoder::
    jcc (this auto&& s, condition c, int32_t o)
    {
      return forward<decltype(s)> (s).encode ([c, o](ZydisEncoderRequest& req)
      {
        req.mnemonic = condition_to_mnemonic (c);
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[0].imm.s = o;
      });
    }

    encoder encoder::
    call (this auto&& s, uint64_t t)
    {
      auto e (forward<decltype(s)> (s).encode ([](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_CALL;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_MEMORY;
        req.operands[0].mem.base = ZYDIS_REGISTER_RIP;
        req.operands[0].mem.displacement = 0;
        req.operands[0].mem.size = 8;
      }));

      auto l (boost::endian::native_to_little (t));
      auto b (reinterpret_cast<uint8_t*> (&l));

      return move (e).append (b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
    }

    encoder encoder::
    call_rel (this auto&& s, int32_t o)
    {
      return forward<decltype(s)> (s).encode ([o](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_CALL;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[0].imm.s = o;
      });
    }

    encoder encoder::
    push (this auto&& s, reg64 re)
    {
      return forward<decltype(s)> (s).encode ([re](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_PUSH;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
        req.operands[0].reg.value = reg64_to_zydis (re);
      });
    }

    encoder encoder::
    pop (this auto&& s, reg64 r)
    {
      return forward<s> (s).encode ([r](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_POP;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
        req.operands[0].reg.value = reg64_to_zydis (r);
      });
    }

    encoder encoder::
    mov (this auto&& s, reg64 r, uint64_t i)
    {
      return forward<delctype(s)> (s).encode ([r, i](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_MOV;
        req.operand_count = 2;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
        req.operands[0].reg.value = reg64_to_zydis (r);
        req.operands[1].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[1].imm.u = i;
      });
    }

    encoder encoder::
    nop (this auto&& s)
    {
      return forward<delctype(s)> (s).append (0x90);
    }

    encoder encoder::
    nop (this auto&& s, size_t n)
    {
      if (n == 0)
        return forward<decltype(s)> (s);

      if (n > 9)
        throw runtime_error ("NOP size exceeds maximum (9 bytes)");

      auto t (forward<decltype(s)> (s).code_.transient ());
      const auto& seq (nop_sequences[n - 1]);

      for (size_t i (0); i < n; ++i)
        t.push_back (seq[i]);

      encoder r;
      r.code_ = t.persistent ();
      return r;
    }

    encoder encoder::
    ret (this auto&& s)
    {
      return forward<decltype(s)> (s).append (0xC3);
    }

    encoder encoder::
    ret (this auto&& s, uint16_t n)
    {
      return forward<decltype(s)> (s).encode ([n](ZydisEncoderRequest& req)
      {
        req.mnemonic = ZYDIS_MNEMONIC_RET;
        req.operand_count = 1;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
        req.operands[0].imm.u = n;
      });
    }

    encoder encoder::
    int3 (this auto&& s)
    {
      return forward<decltype(s)> (s).append (0xCC);
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
