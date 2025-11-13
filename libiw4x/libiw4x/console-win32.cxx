#include <libiw4x/console-win32.hxx>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <ranges>
#include <thread>
#include <utility>

#include <libiw4x/symbol.hxx>

#include <libiw4x/component/network.hxx>
#include <libiw4x/component/party.hxx>

using namespace std;

namespace iw4x
{
  namespace
  {
    using console_show_func = void (*) ();
    using console_output_func = void (*) (const char *);
    using cmd_execute_single_command_func = void (*) (int, int, const char *);

    const console_show_func
      show (reinterpret_cast<console_show_func> (0x1402AB600));

    const console_output_func
      output (reinterpret_cast<console_output_func> (0x1402AAE00));

    const cmd_execute_single_command_func execute (
      reinterpret_cast<cmd_execute_single_command_func> (0x1401ECD60));
  }

  console::console (party& p)
    : party_ (p)
  {
    try
    {
      show ();

      this_thread::sleep_for (chrono::milliseconds (100));

      window_ = FindWindow ("IW4 WinConsole", nullptr);

      if (window_ == nullptr)
        throw console_initialization_error ("console window not found");

      EnumChildWindows (window_,
                        enum_child_proc,
                        reinterpret_cast<LPARAM> (this));

      if (input_ == nullptr)
        throw console_initialization_error ("console input field not found");

      original_wndproc_ = reinterpret_cast<WNDPROC> (
          SetWindowLongPtr (input_,
                            GWLP_WNDPROC,
                            reinterpret_cast<LONG_PTR> (input_wndproc_static)));

      if (original_wndproc_ == nullptr)
        throw console_initialization_error ("failed to install input hook");

      SetWindowLongPtr (input_,
                        GWLP_USERDATA,
                        reinterpret_cast<LONG_PTR> (this));

      register_command ("connect",
                        [&] ()
      {
        string address (args ());
        execute_command ("onlinegame 1");
        println ("Attempting to connect to " + address + "...");

        netadr_t addr;
        if (NET_StringToAdr (address.c_str (), &addr))
        {
          party_.connect_to_party (addr);
          println ("Connect command sent");
        }
        else
        {
          println ("Error: Invalid address format");
        }
      });
    }
    catch (const exception &e)
    {
      throw console_initialization_error (e.what ());
    }
  }

  void
  console::println (const string &message) const
  {
    if (!message.empty ())
      output ((message + "\n").c_str ());
  }

  void
  console::register_command (const string &name, command_handler handler)
  {
    if (name.empty ())
      throw console_command_error ("command name cannot be empty");

    if (!handler)
      throw console_command_error ("command handler cannot be null");

    commands_ [name] = std::move (handler);
  }

  void
  console::unregister_command (const string &name)
  {
    commands_.erase (name);
  }

  bool
  console::command_registered (const string &name) const noexcept
  {
    return commands_.contains (name);
  }

  vector<string>
  console::command_list () const
  {
    vector<string> result;
    result.reserve (commands_.size ());

    ranges::copy (commands_ | ranges::views::keys, back_inserter (result));

    return result;
  }

  void
  console::execute_command (const string &command)
  {
    if (command.empty ())
      return;

    println (command);

    // Parse command and arguments.
    //
    size_t space_pos (command.find (' '));
    string cmd_name (command.substr (0, space_pos));
    string args;

    if (space_pos != string::npos)
      args = command.substr (space_pos + 1);

    auto found (ranges::find_if (commands_,
                                 [&cmd_name] (const auto &pair)
    {
      return pair.first == cmd_name;
    }));

    if (found != commands_.end ())
    {
      try
      {
        // Store args for handlers to access.
        //
        current_args_ = args;
        found->second ();
        current_args_.clear ();
      }
      catch (const exception &e)
      {
        throw console_command_error ("custom command '" + cmd_name +
                                     "': " + e.what ());
      }
      return;
    }

    execute (0, 0, command.c_str ());
  }

  vector<string>
  console::command_history () const
  {
    return history_;
  }

  void
  console::clear_history ()
  {
    history_.clear ();
    history_index_ = -1;
  }

  string
  console::args () const
  {
    return current_args_;
  }

  BOOL CALLBACK
  console::enum_child_proc (HWND hwnd, LPARAM lparam)
  {
    constexpr size_t class_name_size (256);
    array<char, class_name_size> class_name {};

    if (GetClassName (hwnd, class_name.data (), class_name_size) == 0)
      return TRUE;

    if (strcmp (class_name.data (), "Edit") == 0)
    {
      console *impl (reinterpret_cast<console *> (lparam));
      impl->input_ = hwnd;
      return FALSE;
    }

    return TRUE;
  }

  LRESULT CALLBACK
  console::input_wndproc_static (HWND hwnd,
                                 UINT msg,
                                 WPARAM wparam,
                                 LPARAM lparam)
  {
    console *impl (reinterpret_cast<console *> (GetWindowLongPtr (hwnd, GWLP_USERDATA)));

    if (impl != nullptr)
      return impl->input_wndproc (hwnd, msg, wparam, lparam);

    return DefWindowProc (hwnd, msg, wparam, lparam);
  }

  LRESULT
  console::input_wndproc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
  {
    switch (msg)
    {
      case WM_KEYDOWN:
      {
        if (wparam == VK_RETURN)
        {
          constexpr size_t buffer_size (512);
          array<char, buffer_size> buffer {};
          const int length (GetWindowText (hwnd, buffer.data (), buffer_size));

          if (length > 0)
          {
            string command (buffer.data (), static_cast<size_t> (length));

            if (!command.empty () &&
                (history_.empty () || history_.back () != command))
              history_.push_back (command);

            history_index_ = -1;
            SetWindowText (hwnd, "");

            try
            {
              execute_command (command);
            }
            catch (const exception &e)
            {
              cerr << "command don't exist: " << e.what () << '\n';
            }
          }
          return 0;
        }

        // Navigate history backwards.
        //
        if (wparam == VK_UP)
        {
          if (!history_.empty ())
          {
            if (history_index_ == -1)
            {
              history_index_ = static_cast<int> (history_.size ()) - 1;
            }
            else if (std::cmp_greater (history_index_, 0))
            {
              --history_index_;
            }

            if (std::cmp_greater_equal (history_index_, 0) &&
                std::cmp_less (history_index_, history_.size ()))
            {
              SetWindowText (hwnd, history_ [history_index_].c_str ());
              SendMessage (hwnd,
                           EM_SETSEL,
                           static_cast<WPARAM> (-1),
                           static_cast<LPARAM> (-1));
            }
          }
          return 0;
        }

        // Navigate history forwards.
        //
        if (wparam == VK_DOWN)
        {
          if (!history_.empty () && history_index_ != -1)
          {
            ++history_index_;

            if (std::cmp_greater_equal (history_index_, history_.size ()))
            {
              SetWindowText (hwnd, "");
              history_index_ = -1;
            }
            else
            {
              SetWindowText (hwnd, history_ [history_index_].c_str ());
              SendMessage (hwnd,
                           EM_SETSEL,
                           static_cast<WPARAM> (-1),
                           static_cast<LPARAM> (-1));
            }
          }
          return 0;
        }
        break;
      }
    }

    if (original_wndproc_ != nullptr)
      return CallWindowProc (original_wndproc_, hwnd, msg, wparam, lparam);

    return DefWindowProc (hwnd, msg, wparam, lparam);
  }
}
