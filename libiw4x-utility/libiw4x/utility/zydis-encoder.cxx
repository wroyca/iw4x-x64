#include <libiw4x/utility/zydis-encoder.hxx>

#include <algorithm>

#include <Zydis/Zydis.h>

#include <libiw4x/utility/endian.hxx>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    encoder
    encoder::encode (auto&& request)
    {
      ZydisEncoderRequest r {};
      r.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;

      // The caller decides what this instruction should be. We just provide a
      // clean request object and stay out of the way. As long as the fields
      // are filled in, Zydis will take it from there.
      //
      request (r);

      // Perform the encoding. Zydis fills the provided buffer and updates
      // `n` with the number of bytes written. Failure is reported only as a
      // generic status code; the library gives no semantic reason.
      //
      ZyanU8 b [ZYDIS_MAX_INSTRUCTION_LENGTH];
      ZyanUSize n (sizeof (b));

      if (ZYAN_FAILED (ZydisEncoderEncodeInstruction (&r, b, &n)))
        throw encoding_error ("unable to encode instruction");

      return append ({b, n});
    }

    encoder
    encoder::append (span<const uint8_t> instruction)
    {
      instruction::transient_type t (stream.transient ());
      copy (instruction.begin (), instruction.end (), back_inserter (t));

      encoder e;
      e.stream = t.persistent ();
      return e;
    }

    encoder
    encoder::jmp (uint64_t a)
    {
      encoder e (encode ([] (ZydisEncoderRequest& r)
      {
        r.mnemonic = ZYDIS_MNEMONIC_JMP;
        r.operand_count = 1;
        r.operands [0].type = ZYDIS_OPERAND_TYPE_MEMORY;
        r.operands [0].mem.base = ZYDIS_REGISTER_RIP;
        r.operands [0].mem.displacement = 0;
        r.operands [0].mem.size = 8;
      }));

      // Append absolute address in little-endian format.
      //
      uint8_t b [8];
      store_le (b, a);
      return std::move (e).append ({b, 8});
    }
  }
}
