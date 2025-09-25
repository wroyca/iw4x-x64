#pragma once

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <libiw4x/utility/utility-win32.hxx>

namespace iw4x
{
  using command_handler = std::function<void ()>;

  struct console_error : std::runtime_error
  {
    explicit console_error (const std::string &w)
      : std::runtime_error (w) {}
  };

  struct console_initialization_error : console_error
  {
    explicit console_initialization_error (const std::string &w)
      : console_error ("console initialization failed: " + w) {}
  };

  struct console_command_error : console_error
  {
    explicit console_command_error (const std::string &w)
      : console_error ("console command failed: " + w) {}
  };

  class console
  {
  public:
    console ();

    void
    println (const std::string &message) const;

    void
    register_command (const std::string &name, command_handler handler);

    void
    unregister_command (const std::string &name);

    bool
    command_registered (const std::string &name) const noexcept;

    std::vector<std::string>
    command_list () const;

    void
    execute_command (const std::string &command);

    std::vector<std::string>
    command_history () const;

    void
    clear_history ();

  private:
    HWND window_ = nullptr;
    HWND input_ = nullptr;
    WNDPROC original_wndproc_ = nullptr;

    std::vector<std::string> history_;
    int history_index_ = -1;
    std::unordered_map<std::string, command_handler> commands_;

    static BOOL CALLBACK
    enum_child_proc (HWND hwnd, LPARAM lparam);

    static LRESULT CALLBACK
    input_wndproc_static (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    LRESULT
    input_wndproc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
  };

  void
  show_console ();
}
