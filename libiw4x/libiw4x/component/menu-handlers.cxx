#include <libiw4x/component/menu-handlers.hxx>

namespace iw4x
{
  menu_handlers::menu_handlers ()
  {
    ([] (auto&& action, auto&& expression)
    {
      action
      (
        "main_text",
        "@PLATFORM_PLAY_ONLINE_CAPS",
        {
          "play mouse_click",
          "open menu_xboxlive"
        }
      );

      action
      (
        "menu_xboxlive",
        "@MENU_PRIVATE_MATCH_CAPS",
        {
          "play mouse_click",
          "exec xcheckezpatch",
          "exec default_xboxlive.cfg",
          "exec xblive_rankedmatch 0",
          "exec ui_enumeratesaved exec xblive_privatematch 1",
          "exec onlinegame 0",
          "exec xblive_hostingprivateparty 1",
          "exec xblive_privatepartyclient 1",
          "exec xstartprivatematch",
          "open menu_xboxlive_privatelobby"
        }
      );

      expression
      (
        "menu_xboxlive_privatelobby",
        "@MENU_START_GAME_CAPS"
      );

      expression
      (
        "menu_xboxlive_privatelobby",
        "@MENU_GAME_SETUP_CAPS"
      );
    })

    // Action handler.
    //
    ([this] (const string& mn, const string& in, const vector<string>& cmd)
    {
      menuDef_t* m (find_menu (mn));
      itemDef_s* i (find_item (m, in));

      MenuEventHandlerSet* hs (make_handler_set (cmd));
      i->action = hs;
    },

    // Expression handler.
    //
    // Note that this handler currently only nullifies the disabled expression
    // property, effectively enabling the menu item unconditionally. We don't
    // handle or allow any other expression modifications at this time.
    //
    [this] (const string& mn, const string& in)
    {
      menuDef_t* m (find_menu (mn));
      itemDef_s* i (find_item (m, in));

      i->disabledExp = nullptr;
    });
  }

  menuDef_t*
  menu_handlers::find_menu (const string& n)
  {
    XAssetHeader h (DB_FindXAssetHeader (ASSET_TYPE_MENU, n.c_str()));

    return h.menu;
  }

  itemDef_s*
  menu_handlers::find_item (menuDef_t* m, const string& name)
  {
    for (int i (0); i < m->itemCount; ++i)
    {
      itemDef_s* item (m->items [i]);

      // Note that we have no guarantee that the engine will always match item
      // count with the actual items array size.
      //
      if ((item != nullptr) && (item->text != nullptr) && item->text == name)
        return item;
    }

    throw runtime_error (format ("unable to resolve menu item: {}", name));
  }

  MenuEventHandlerSet*
  menu_handlers::make_handler_set (const vector<string>& cmds)
  {
    MenuEventHandlerSet* hs (new MenuEventHandlerSet ());
    hs->eventHandlerCount = static_cast<int> (cmds.size ());
    hs->eventHandlers = new MenuEventHandler*[cmds.size ()];

    // Index is needed for array assignment.
    //
    for (size_t i (0); const auto& cmd : cmds)
    {
      MenuEventHandler* h (make_handler (cmd, event_type::ev_unconditional));
      hs->eventHandlers [i++] = h;
    }

    return hs;
  }

  MenuEventHandler*
  menu_handlers::make_handler (const string& cmd, event_type t)
  {
    MenuEventHandler* h (new MenuEventHandler ());
    h->eventType = static_cast<char> (t);

    switch (t)
    {
      case event_type::ev_unconditional:
        h->eventData.unconditionalScript = _strdup (cmd.c_str ());
        break;
      case event_type::ev_if:
      case event_type::ev_else:
      case event_type::ev_set_local_var_bool:
      case event_type::ev_set_local_var_int:
      case event_type::ev_set_local_var_float:
      case event_type::ev_set_local_var_string:
        h->eventData.unconditionalScript = _strdup (cmd.c_str ());
        break;
    }

    return h;
  }
}
