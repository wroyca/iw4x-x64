#include <cassert>
#include <cstdint>
#include <iostream>

#include <libiw4x/utility/zydis-encoder.hxx>

#undef NDEBUG
#include <cassert>

int
main ()
{
  using namespace iw4x::utility;

  // Helper to assert two instruction sequences are equal.
  //
  auto is_eq ([] (const instruction& seq1, const instruction& seq2)
  {
    assert (seq1.size () == seq2.size ());
    for (size_t i (0); i < seq1.size (); ++i)
      assert (seq1 [i] == seq2 [i]);
  });

  // Test encoder initialization
  //
  {
    encoder e;
    assert (e.empty ());
    assert (e.size () == 0);
  }

  // Test jmp instruction encoding
  //
  {
    encoder e;
    e = e.jmp (0x123456789abcdef0);

    assert (!e.empty ());
    assert (e.size () > 0);

    // A RIP-relative indirect jump should be:
    // - 6 bytes for the jmp [rip+0] instruction
    // - 8 bytes for the 64-bit absolute address
    //
    assert (e.size () == 14);

    instruction seq (e.emit ());
    assert (seq.size () == 14);

    // First byte should be 0xFF (jmp opcode).
    //
    assert (seq[0] == 0xFF);

    // The last 8 bytes should be the little-endian encoded address.
    //
    assert (seq[6] == 0xf0);
    assert (seq[7] == 0xde);
    assert (seq[8] == 0xbc);
    assert (seq[9] == 0x9a);
    assert (seq[10] == 0x78);
    assert (seq[11] == 0x56);
    assert (seq[12] == 0x34);
    assert (seq[13] == 0x12);
  }

  // Test multiple jmp instructions
  //
  {
    encoder e1;
    e1 = e1.jmp (0x1111111111111111);

    encoder e2;
    e2 = e2.jmp (0x2222222222222222);

    assert (e1.size () == 14);
    assert (e2.size () == 14);

    instruction seq1 (e1.emit ());
    instruction seq2 (e2.emit ());

    // Both should be same size
    //
    assert (seq1.size () == seq2.size ());

    // But different addresses
    //
    assert (seq1[6] == 0x11);
    assert (seq2[6] == 0x22);
  }

  // Test encoder immutability
  //
  {
    encoder e1;
    encoder e2 (e1.jmp (0x4000000000000000));

    // e1 should still be empty
    //
    assert (e1.empty ());
    assert (e1.size () == 0);

    // e2 should have the instruction
    //
    assert (!e2.empty ());
    assert (e2.size () == 14);
  }

  // Test emit is non-destructive
  //
  {
    encoder e;
    e = e.jmp (0x5000000000000000);

    instruction seq1 (e.emit ());
    instruction seq2 (e.emit ());

    // Both should be identical
    //
    is_eq (seq1, seq2);
  }

  // Test various jump addresses
  //
  {
    // Min address
    //
    encoder e1;
    e1 = e1.jmp (0x0000000000000000);
    assert (e1.size () == 14);
    instruction seq1 (e1.emit ());
    for (size_t i (6); i < 14; ++i)
      assert (seq1[i] == 0x00);

    // Max address
    //
    encoder e2;
    e2 = e2.jmp (0xFFFFFFFFFFFFFFFF);
    assert (e2.size () == 14);
    instruction seq2 (e2.emit ());
    for (size_t i (6); i < 14; ++i)
      assert (seq2[i] == 0xFF);

    // Mid-range address
    //
    encoder e3;
    e3 = e3.jmp (0x00007FF000000000);
    assert (e3.size () == 14);
    instruction seq3 (e3.emit ());
    assert (seq3[6] == 0x00);
    assert (seq3[7] == 0x00);
    assert (seq3[8] == 0x00);
    assert (seq3[9] == 0x00);
    assert (seq3[10] == 0xF0);
    assert (seq3[11] == 0x7F);
    assert (seq3[12] == 0x00);
    assert (seq3[13] == 0x00);
  }

  // Test persistent vector semantics
  //
  {
    encoder e;
    e = e.jmp (0x6000000000000000);

    instruction seq1 (e.emit ());
    instruction seq2 (seq1); // Copy

    // Both should be equal
    //
    is_eq (seq1, seq2);
  }
}
