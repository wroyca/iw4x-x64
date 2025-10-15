#include <libiw4x/database-menu-handlers.hxx>

#include <sstream>
#include <mutex>

using namespace std;

namespace iw4x
{
  namespace
  {
    once_flag menu_handlers_init;
  }

  menu_handlers::menu_handlers (renderer& r)
  {
    // FIXME: need scheduler
    //
    r.on_frame ().connect ([this] (IDirect3DDevice9* d)
    {
      call_once (menu_handlers_init, [this]
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
        ([this] (const string& menu_name,
                 const string& item_name,
                 const vector<string>& commands)
        {
          menuDef_t* menu (find_menu (menu_name));
          itemDef_s* item (find_item (menu, item_name));

          MenuEventHandlerSet* handler_set (make_handler_set (commands));
          item->action = handler_set;
        },

        // Expression handler.
        //
        // Note that this handler currently only nullifies the disabled
        // expression property, effectively enabling the menu item
        // unconditionally. We don't handle or allow any other expression
        // modifications at this time.
        //
        [this] (const string& menu_name, const string& item_name)
        {
          menuDef_t* menu (find_menu (menu_name));
          itemDef_s* item (find_item (menu, item_name));

          item->disabledExp = nullptr;
        });
      });
    });
  }

  menuDef_t*
  menu_handlers::find_menu (const string& name)
  {
    XAssetHeader header (DB_FindXAssetHeader (ASSET_TYPE_MENU, name.c_str()));

    return header.menu;
  }

  itemDef_s*
  menu_handlers::find_item (menuDef_t* menu, const string& name)
  {
    for (int i (0); i < menu->itemCount; ++i)
    {
      itemDef_s* item (menu->items [i]);

      // Note that we have no guarantee that the engine will always match item
      // count with the actual items array size.
      //
      if ((item != nullptr) && (item->text != nullptr) && item->text == name)
        return item;
    }

    ostringstream s;

    s << "unable to resolve menu item: "
      << name << endl;

    throw runtime_error (s.str ());
  }

  MenuEventHandlerSet*
  menu_handlers::make_handler_set (const vector<string>& commands)
  {
    MenuEventHandlerSet* handler_set (new MenuEventHandlerSet ());
    handler_set->eventHandlerCount = static_cast<int> (commands.size ());
    handler_set->eventHandlers = new MenuEventHandler*[commands.size ()];

    // Index is needed for array assignment.
    //
    for (size_t i (0); const auto& command : commands)
    {
      MenuEventHandler* handler (make_handler (command, event_type::ev_unconditional));
      handler_set->eventHandlers [i++] = handler;
    }

    return handler_set;
  }

  MenuEventHandler*
  menu_handlers::make_handler (const string& command, event_type type)
  {
    MenuEventHandler* handler (new MenuEventHandler ());
    handler->eventType = static_cast<char> (type);

    switch (type)
    {
      case event_type::ev_unconditional:
        handler->eventData.unconditionalScript = _strdup (command.c_str ());
        break;
      case event_type::ev_if:
      case event_type::ev_else:
      case event_type::ev_set_local_var_bool:
      case event_type::ev_set_local_var_int:
      case event_type::ev_set_local_var_float:
      case event_type::ev_set_local_var_string:
        handler->eventData.unconditionalScript = _strdup (command.c_str ());
        break;
    }

    return handler;
  }
}
