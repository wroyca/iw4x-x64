#include <libiw4x/mod/console.hxx>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <ranges>
#include <thread>
#include <utility>

#include <libiw4x/mod/party.hxx>
#include <libiw4x/sym/cmd.hxx>
#include <libiw4x/sym/console.hxx>
#include <libiw4x/sym/net.hxx>

using namespace std;

namespace iw4x
{
  namespace mod
  {
    console::console (party& p)
      : party_ (p)
    {
      try
      {
        console_show ();
        this_thread::sleep_for (chrono::milliseconds (100));

        window_ = FindWindow ("IW4 WinConsole", nullptr);
        if (!window_)
          throw console_initialization_error ("console window not found");

        EnumChildWindows (window_,
                          enum_child_proc,
                          reinterpret_cast<LPARAM> (this));
        if (!input_)
          throw console_initialization_error ("console input field not found");

        auto wndproc (
          SetWindowLongPtr (input_,
                            GWLP_WNDPROC,
                            reinterpret_cast<LONG_PTR> (input_wndproc_static)));
        if (!wndproc)
          throw console_initialization_error ("failed to install input hook");

        original_wndproc_ = reinterpret_cast<WNDPROC> (wndproc);
        SetWindowLongPtr (input_,
                          GWLP_USERDATA,
                          reinterpret_cast<LONG_PTR> (this));

        register_command ("connect",
                          [&] ()
        {
          const string address (args ());
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
      catch (const exception& e)
      {
        throw console_initialization_error (e.what ());
      }
    }

    void
    console::println (const string& m) const
    {
      if (!m.empty ())
        console_output ((m + "\n").c_str ());
    }

    void
    console::register_command (const string& n, command_handler h)
    {
      if (n.empty ())
        throw console_command_error ("command name cannot be empty");
      if (!h)
        throw console_command_error ("command handler cannot be null");

      commands_ [n] = move (h);
    }

    void
    console::unregister_command (const string& n)
    {
      commands_.erase (n);
    }

    bool
    console::command_registered (const string& n) const noexcept
    {
      return commands_.contains (n);
    }

    vector<string>
    console::command_list () const
    {
      vector<string> out;
      out.reserve (commands_.size ());

      for (const auto& [k, _] : commands_)
        out.push_back (k);

      return out;
    }

    void
    console::execute_command (const string& c)
    {
      if (c.empty ())
        return;

      println (c);

      const auto pos (c.find (' '));
      const string cmd (pos == string::npos ? c : c.substr (0, pos));
      const string arg (pos == string::npos ? "" : c.substr (pos + 1));

      if (const auto it (commands_.find (cmd)); it != commands_.end ())
      {
        try
        {
          current_args_ = arg;
          it->second ();
          current_args_.clear ();
        }
        catch (const exception& e)
        {
          throw console_command_error ("custom command '" + cmd +
                                       "': " + e.what ());
        }

        return;
      }

      cmd_execute_single_command (0, 0, c.c_str ());
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
      constexpr size_t n (256);
      array<char, n> cls {};

      if (!GetClassName (hwnd, cls.data (), n))
        return TRUE;

      if (strcmp (cls.data (), "Edit") == 0)
      {
        reinterpret_cast<console*> (lparam)->input_ = hwnd;
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
      console* self (
        reinterpret_cast<console*> (GetWindowLongPtr (hwnd, GWLP_USERDATA)));

      if (self != nullptr)
        return self->input_wndproc (hwnd, msg, wparam, lparam);

      return DefWindowProc (hwnd, msg, wparam, lparam);
    }

    LRESULT
    console::input_wndproc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
      if (msg == WM_KEYDOWN)
      {
        if (wparam == VK_RETURN)
        {
          array<char, 512> buf {};
          const int len (GetWindowText (hwnd, buf.data (), buf.size ()));

          if (len > 0)
          {
            const string line (buf.data (), static_cast<size_t> (len));

            if (!line.empty () &&
                (history_.empty () || history_.back () != line))
              history_.push_back (line);

            history_index_ = -1;
            SetWindowText (hwnd, "");

            try
            {
              execute_command (line);
            }
            catch (const exception& e)
            {
              cerr << "command don't exist: " << e.what () << '\n';
            }
          }
          return 0;
        }

        if (wparam == VK_UP)
        {
          if (!history_.empty ())
          {
            if (history_index_ < 0)
              history_index_ = static_cast<int> (history_.size ()) - 1;
            else if (history_index_ > 0)
              --history_index_;

            const auto idx (history_index_);
            if (idx >= 0 && static_cast<size_t> (idx) < history_.size ())
            {
              SetWindowText (hwnd, history_ [idx].c_str ());
              SendMessage (hwnd, EM_SETSEL, -1, -1);
            }
          }
          return 0;
        }

        if (wparam == VK_DOWN)
        {
          if (!history_.empty () && history_index_ != -1)
          {
            ++history_index_;
            const auto idx (history_index_);

            if (static_cast<size_t> (idx) >= history_.size ())
            {
              SetWindowText (hwnd, "");
              history_index_ = -1;
            }
            else
            {
              SetWindowText (hwnd, history_ [idx].c_str ());
              SendMessage (hwnd, EM_SETSEL, -1, -1);
            }
          }
          return 0;
        }
      }

      if (original_wndproc_)
        return CallWindowProc (original_wndproc_, hwnd, msg, wparam, lparam);

      return DefWindowProc (hwnd, msg, wparam, lparam);
    }
  }
}
