#include <libiw4x/utility/xxhash.hxx>

#include <xxhash.h>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    uint64_t
    xxh64 (const string& in)
    {
      return XXH64 (in.data (), in.size (), 0);
    }

    uint64_t
    xxh64 (const string& in, uint64_t s)
    {
      return XXH64 (in.data (), in.size (), s);
    }

    uint64_t
    xxh64 (span<const byte> in)
    {
      return XXH64 (in.data (), in.size (), 0);
    }

    uint64_t
    xxh64 (span<const byte> in, uint64_t s)
    {
      return XXH64 (in.data (), in.size (), s);
    }

    // xxh64_hasher
    //

    struct xxh64_hasher::state
    {
      XXH64_state_t* st;

      state ()
          : st (XXH64_createState ())
      {
      }

      ~state ()
      {
        if (st != nullptr)
          XXH64_freeState (st);
      }
    };

    xxh64_hasher::
    xxh64_hasher ()
        : state_ (new state ())
    {
      XXH64_reset (state_->st, 0);
    }

    xxh64_hasher::
    xxh64_hasher (uint64_t s)
        : state_ (new state ())
    {
      XXH64_reset (state_->st, s);
    }

    xxh64_hasher::
    ~xxh64_hasher ()
    {
      delete state_;
    }

    void xxh64_hasher::
    reset ()
    {
      XXH64_reset (state_->st, 0);
    }

    void xxh64_hasher::
    reset (uint64_t s)
    {
      XXH64_reset (state_->st, s);
    }

    void xxh64_hasher::
    update (const string& in)
    {
      XXH64_update (state_->st, in.data (), in.size ());
    }

    void xxh64_hasher::
    update (span<const byte> in)
    {
      XXH64_update (state_->st, in.data (), in.size ());
    }

    uint64_t xxh64_hasher::
    digest () const
    {
      return XXH64_digest (state_->st);
    }
  }
}
