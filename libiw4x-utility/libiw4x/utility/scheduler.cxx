#include <libiw4x/utility/scheduler.hxx>

#include <stdexcept>
#include <utility>

#include <libiw4x/utility/xxhash.hxx>

using namespace std;

namespace iw4x
{
  namespace utility
  {
    // scheduler::pipeline_context
    //

    scheduler::pipeline_context::
    pipeline_context (const string& n)
        : name (n), tasks (), mtx ()
    {
    }

    scheduler::pipeline_context::
    ~pipeline_context ()
    {
    }

    void scheduler::pipeline_context::
    enqueue (function<void ()> t)
    {
      lock_guard<mutex> lck (mtx);
      tasks.push_back (move (t));
    }

    vector<function<void ()>> scheduler::pipeline_context::
    extract_pending ()
    {
      vector<function<void ()>> pnd;

      {
        lock_guard<mutex> lck (mtx);
        pnd.swap (tasks);
      }

      return pnd;
    }

    bool scheduler::pipeline_context::
    has_pending () const
    {
      lock_guard<mutex> lck (mtx);
      return !tasks.empty ();
    }

    void scheduler::pipeline_context::
    clear ()
    {
      lock_guard<mutex> lck (mtx);
      tasks.clear ();
    }

    // scheduler
    //

    scheduler::
    scheduler ()
      : pipelines_ (), registry_mutex_ ()
    {
    }

    scheduler::
    ~scheduler ()
    {
      lock_guard<mutex> lck (registry_mutex_);
      pipelines_.clear ();
    }

    void scheduler::
    register_pipeline (const string& n)
    {
      if (n.empty ())
        throw invalid_argument ("pipeline name cannot be empty");

      uint64_t h (xxh64 (n));
      lock_guard<mutex> lck (registry_mutex_);

      // Check for duplicate names.
      //
      if (pipelines_.find (h) != pipelines_.end ())
        throw invalid_argument ("pipeline name already registered: " + n);

      pipelines_.emplace (h, make_unique<pipeline_context> (n));
    }

    void scheduler::
    unregister_pipeline (const string& n)
    {
      if (n.empty ())
        throw invalid_argument ("pipeline name cannot be empty");

      uint64_t h (xxh64 (n));
      lock_guard<mutex> lck (registry_mutex_);

      if (pipelines_.find (h) == pipelines_.end ())
        throw invalid_argument ("pipeline not registered: " + n);

      pipelines_.erase (h);
    }

    void scheduler::
    poll (const string& n)
    {
      uint64_t h (xxh64 (n));
      pipeline_context* ctx (find_context (h));

      if (ctx == nullptr)
        throw invalid_argument ("pipeline not registered: " + n);

      process_tasks (*ctx);
    }

    void scheduler::
    enqueue_task (const string& n, function<void ()> f)
    {
      uint64_t h (xxh64 (n));
      pipeline_context* ctx (find_context (h));

      if (ctx == nullptr)
        throw invalid_argument ("pipeline not registered: " + n);

      ctx->enqueue (move (f));
    }

    bool scheduler::
    has_pending (const string& n) const
    {
      uint64_t h (xxh64 (n));
      const pipeline_context* ctx (find_context (h));

      if (ctx == nullptr)
        throw invalid_argument ("pipeline not registered: " + n);

      return ctx->has_pending ();
    }

    bool scheduler::
    is_registered (const string& n) const
    {
      uint64_t h (xxh64 (n));
      lock_guard<mutex> lck (registry_mutex_);
      return pipelines_.find (h) != pipelines_.end ();
    }

    scheduler::pipeline_context* scheduler::
    find_context (uint64_t h) noexcept
    {
      lock_guard<mutex> lck (registry_mutex_);

      unordered_map<uint64_t, unique_ptr<pipeline_context>>::iterator it (
        pipelines_.find (h));

      return it != pipelines_.end () ? it->second.get () : nullptr;
    }

    const scheduler::pipeline_context* scheduler::
    find_context (uint64_t h) const noexcept
    {
      lock_guard<mutex> lck (registry_mutex_);

      unordered_map<uint64_t, unique_ptr<pipeline_context>>::const_iterator it (
        pipelines_.find (h));

      return it != pipelines_.end () ? it->second.get () : nullptr;
    }

    void scheduler::
    process_tasks (pipeline_context& ctx)
    {
      vector<function<void ()>> tsks (ctx.extract_pending ());

      // Execute tasks outside the lock.
      //
      for (function<void ()>& tsk : tsks)
        tsk ();
    }
  }
}
