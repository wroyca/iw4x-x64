#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <libiw4x/utility/export.hxx>

namespace iw4x
{
  namespace utility
  {
    class LIBIW4X_UTILITY_SYMEXPORT scheduler
    {
    public:
      scheduler ();
      ~scheduler ();

      scheduler (const scheduler&) = delete;
      scheduler& operator= (const scheduler&) = delete;

      // Register a new pipeline with the given name.
      //
      // Pipeline names must be unique. Throws std::invalid_argument if the
      // name is empty or already registered.
      //
      void
      register_pipeline (const std::string& name);

      // Unregister a pipeline, clearing any pending tasks.
      //
      // Throws std::invalid_argument if the pipeline is not registered.
      //
      void
      unregister_pipeline (const std::string& name);

      // Poll one pipeline to process pending tasks.
      //
      // This is non-blocking and processes all ready tasks. Throws
      // std::invalid_argument if the pipeline is not registered.
      //
      void
      poll (const std::string& name);

      // Post an immediate task to the specified pipeline.
      //
      // The task is enqueued and will execute when the pipeline is polled.
      // Throws std::invalid_argument if the pipeline is not registered.
      //
      template <typename F>
      void
      post (const std::string& pipeline_name, F&& function)
      {
        enqueue_task (pipeline_name,
                      std::function<void ()> (static_cast<F&&> (function)));
      }

      // Check if a pipeline has pending tasks.
      //
      // Throws std::invalid_argument if the pipeline is not registered.
      //
      bool
      has_pending (const std::string& name) const;

      // Check if a pipeline is registered.
      //
      bool
      is_registered (const std::string& name) const;

    private:
      // Pipeline context holds the task queue for a single pipeline.
      //
      struct pipeline_context
      {
        std::string name;
        std::vector<std::function<void ()>> tasks;
        mutable std::mutex mtx;

        explicit
        pipeline_context (const std::string& pipeline_name);

       ~pipeline_context ();

        pipeline_context (const pipeline_context&) = delete;
        pipeline_context& operator= (const pipeline_context&) = delete;

        void
        enqueue (std::function<void ()> task);

        std::vector<std::function<void ()>>
        extract_pending ();

        bool
        has_pending () const;

        void
        clear ();
      };

      std::unordered_map<std::uint64_t, std::unique_ptr<pipeline_context>> pipelines_;
      mutable std::mutex registry_mutex_;

      void
      enqueue_task (const std::string& name, std::function<void ()>);

      pipeline_context*
      find_context (std::uint64_t hash) noexcept;

      const pipeline_context*
      find_context (std::uint64_t hash) const noexcept;

      void
      process_tasks (pipeline_context&);
    };
  }
}
