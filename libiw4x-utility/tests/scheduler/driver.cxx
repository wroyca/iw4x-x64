#include <stdexcept>
#include <string>

#include <libiw4x/utility/scheduler.hxx>

#undef NDEBUG
#include <cassert>

int main ()
{
  using namespace std;
  using namespace iw4x::utility;

  // Test register_pipeline
  //
  {
    scheduler sched;
    sched.register_pipeline ("test_pipeline");

    // Verify pipeline is registered
    //
    assert (sched.is_registered ("test_pipeline"));

    // Verify different name is not registered
    //
    assert (!sched.is_registered ("other_pipeline"));
  }

  // Test register_pipeline with empty name
  //
  {
    scheduler sched;

    try
    {
      sched.register_pipeline ("");
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (e.what () == string ("pipeline name cannot be empty"));
    }
  }

  // Test register_pipeline with duplicate name
  //
  {
    scheduler sched;
    sched.register_pipeline ("duplicate");

    try
    {
      sched.register_pipeline ("duplicate");
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (e.what () == string ("pipeline name already registered: duplicate"));
    }
  }

  // Test unregister_pipeline
  //
  {
    scheduler sched;
    sched.register_pipeline ("temp");
    assert (sched.is_registered ("temp"));

    sched.unregister_pipeline ("temp");
    assert (!sched.is_registered ("temp"));
  }

  // Test unregister_pipeline with empty name
  //
  {
    scheduler sched;

    try
    {
      sched.unregister_pipeline ("");
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (e.what () == string ("pipeline name cannot be empty"));
    }
  }

  // Test unregister_pipeline with non-existent pipeline
  //
  {
    scheduler sched;

    try
    {
      sched.unregister_pipeline ("nonexistent");
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (e.what () == string ("pipeline not registered: nonexistent"));
    }
  }

  // Test post and poll with single task
  //
  {
    scheduler sched;
    sched.register_pipeline ("main");

    int counter (0);

    sched.post ("main", [&counter] () { ++counter; });

    // Task should not execute until poll
    //
    assert (counter == 0);
    assert (sched.has_pending ("main"));

    sched.poll ("main");

    // Task should have executed
    //
    assert (counter == 1);
    assert (!sched.has_pending ("main"));
  }

  // Test post and poll with multiple tasks
  //
  {
    scheduler sched;
    sched.register_pipeline ("main");

    int sum (0);

    sched.post ("main", [&sum] () { sum += 1; });
    sched.post ("main", [&sum] () { sum += 2; });
    sched.post ("main", [&sum] () { sum += 3; });

    assert (sched.has_pending ("main"));

    sched.poll ("main");

    // All tasks should have executed
    //
    assert (sum == 6);
    assert (!sched.has_pending ("main"));
  }

  // Test post to unregistered pipeline
  //
  {
    scheduler sched;

    try
    {
      sched.post ("missing", [] () {});
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (e.what () == string ("pipeline not registered: missing"));
    }
  }

  // Test poll on unregistered pipeline
  //
  {
    scheduler sched;

    try
    {
      sched.poll ("missing");
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (e.what () == string ("pipeline not registered: missing"));
    }
  }

  // Test has_pending on unregistered pipeline
  //
  {
    scheduler sched;

    try
    {
      sched.has_pending ("missing");
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (e.what () == string ("pipeline not registered: missing"));
    }
  }

  // Test poll with no pending tasks
  //
  {
    scheduler sched;
    sched.register_pipeline ("empty");

    assert (!sched.has_pending ("empty"));

    // Should not throw, just do nothing
    //
    sched.poll ("empty");

    assert (!sched.has_pending ("empty"));
  }

  // Test multiple pipelines
  //
  {
    scheduler sched;
    sched.register_pipeline ("pipeline_a");
    sched.register_pipeline ("pipeline_b");

    int counter_a (0);
    int counter_b (0);

    sched.post ("pipeline_a", [&counter_a] () { ++counter_a; });
    sched.post ("pipeline_b", [&counter_b] () { ++counter_b; });

    // Both should have pending tasks
    //
    assert (sched.has_pending ("pipeline_a"));
    assert (sched.has_pending ("pipeline_b"));

    // Poll only pipeline_a
    //
    sched.poll ("pipeline_a");

    assert (counter_a == 1);
    assert (counter_b == 0);
    assert (!sched.has_pending ("pipeline_a"));
    assert (sched.has_pending ("pipeline_b"));

    // Poll pipeline_b
    //
    sched.poll ("pipeline_b");

    assert (counter_a == 1);
    assert (counter_b == 1);
    assert (!sched.has_pending ("pipeline_b"));
  }

  // Test task execution order (FIFO)
  //
  {
    scheduler sched;
    sched.register_pipeline ("ordered");

    string result;

    sched.post ("ordered", [&result] () { result += "A"; });
    sched.post ("ordered", [&result] () { result += "B"; });
    sched.post ("ordered", [&result] () { result += "C"; });

    sched.poll ("ordered");

    assert (result == "ABC");
  }

  // Test unregister clears pending tasks
  //
  {
    scheduler sched;
    sched.register_pipeline ("temp");

    int counter (0);
    sched.post ("temp", [&counter] () { ++counter; });

    assert (sched.has_pending ("temp"));

    sched.unregister_pipeline ("temp");

    // Pipeline is gone, counter should not have changed
    //
    assert (counter == 0);
    assert (!sched.is_registered ("temp"));
  }

  // Test re-registering after unregister
  //
  {
    scheduler sched;
    sched.register_pipeline ("reuse");

    int counter (0);
    sched.post ("reuse", [&counter] () { ++counter; });

    sched.unregister_pipeline ("reuse");
    sched.register_pipeline ("reuse");

    // New pipeline should have no pending tasks
    //
    assert (!sched.has_pending ("reuse"));

    sched.post ("reuse", [&counter] () { ++counter; });
    sched.poll ("reuse");

    // Only the second task should have executed
    //
    assert (counter == 1);
  }

  // Test post with capturing lambda
  //
  {
    scheduler sched;
    sched.register_pipeline ("capture");

    int value (42);
    bool executed (false);

    sched.post ("capture", [&executed, value] ()
    {
      executed = true;
      assert (value == 42);
    });

    sched.poll ("capture");
    assert (executed);
  }

  // Test post with mutable lambda
  //
  {
    scheduler sched;
    sched.register_pipeline ("mutable");

    int external (0);

    sched.post ("mutable", [&external] () mutable
    {
      external = 100;
    });

    sched.poll ("mutable");
    assert (external == 100);
  }

  // Test multiple polls on same pipeline
  //
  {
    scheduler sched;
    sched.register_pipeline ("multi_poll");

    int counter (0);

    sched.post ("multi_poll", [&counter] () { ++counter; });
    sched.poll ("multi_poll");
    assert (counter == 1);

    sched.post ("multi_poll", [&counter] () { ++counter; });
    sched.poll ("multi_poll");
    assert (counter == 2);

    sched.post ("multi_poll", [&counter] () { ++counter; });
    sched.poll ("multi_poll");
    assert (counter == 3);
  }

  // Test post after poll
  //
  {
    scheduler sched;
    sched.register_pipeline ("continuous");

    int counter (0);

    sched.post ("continuous", [&counter] () { ++counter; });
    sched.poll ("continuous");

    assert (counter == 1);
    assert (!sched.has_pending ("continuous"));

    sched.post ("continuous", [&counter] () { ++counter; });

    assert (sched.has_pending ("continuous"));

    sched.poll ("continuous");

    assert (counter == 2);
  }

  // Test task that posts another task
  //
  {
    scheduler sched;
    sched.register_pipeline ("recursive");

    int counter (0);

    sched.post ("recursive", [&sched, &counter] ()
    {
      ++counter;
      sched.post ("recursive", [&counter] () { ++counter; });
    });

    sched.poll ("recursive");

    // First task executed, second task posted but not yet polled
    //
    assert (counter == 1);
    assert (sched.has_pending ("recursive"));

    sched.poll ("recursive");

    // Second task executed
    //
    assert (counter == 2);
  }

  // Test pipeline names are case-sensitive
  //
  {
    scheduler sched;
    sched.register_pipeline ("CaseSensitive");
    sched.register_pipeline ("casesensitive");

    assert (sched.is_registered ("CaseSensitive"));
    assert (sched.is_registered ("casesensitive"));

    // These are different pipelines
    //
    int counter_upper (0);
    int counter_lower (0);

    sched.post ("CaseSensitive", [&counter_upper] () { ++counter_upper; });
    sched.post ("casesensitive", [&counter_lower] () { ++counter_lower; });

    sched.poll ("CaseSensitive");
    assert (counter_upper == 1);
    assert (counter_lower == 0);

    sched.poll ("casesensitive");
    assert (counter_upper == 1);
    assert (counter_lower == 1);
  }

  // Test pipeline with special characters
  //
  {
    scheduler sched;
    sched.register_pipeline ("pipe-line_123!@#");

    assert (sched.is_registered ("pipe-line_123!@#"));

    int counter (0);
    sched.post ("pipe-line_123!@#", [&counter] () { ++counter; });
    sched.poll ("pipe-line_123!@#");

    assert (counter == 1);
  }

  // Test has_pending returns false after poll
  //
  {
    scheduler sched;
    sched.register_pipeline ("check_pending");

    sched.post ("check_pending", [] () {});

    assert (sched.has_pending ("check_pending"));

    sched.poll ("check_pending");

    assert (!sched.has_pending ("check_pending"));
  }

  // Test exception safety in tasks
  //
  {
    scheduler sched;
    sched.register_pipeline ("exception_test");

    int counter (0);

    sched.post ("exception_test", [&counter] () { ++counter; });
    sched.post ("exception_test", [] () { throw runtime_error ("test exception"); });
    sched.post ("exception_test", [&counter] () { ++counter; });

    // Poll should execute first task, throw on second, and not reach third
    //
    try
    {
      sched.poll ("exception_test");
      assert (false);
    }
    catch (const runtime_error& e)
    {
      assert (e.what () == string ("test exception"));
    }

    // First task executed, third task never reached
    //
    assert (counter == 1);
  }

  // Test long pipeline names
  //
  {
    scheduler sched;
    string long_name (1000, 'x');
    sched.register_pipeline (long_name);

    assert (sched.is_registered (long_name));

    int counter (0);
    sched.post (long_name, [&counter] () { ++counter; });
    sched.poll (long_name);

    assert (counter == 1);
  }

  // Test destructor clears all pipelines
  //
  {
    int counter (0);

    {
      scheduler sched;
      sched.register_pipeline ("temp");
      sched.post ("temp", [&counter] () { ++counter; });

      // Scheduler destroyed here, tasks not executed
    }

    assert (counter == 0);
  }
}
