#include <libiw4x/component/menu.hxx>

#include <libiw4x/symbol.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  namespace
  {
    menuDef_t&
    find_menu (const string& name)
    {
      XAssetHeader header (DB_FindXAssetHeader (ASSET_TYPE_MENU, name.c_str()));

      if (header.menu == nullptr)
        throw runtime_error ("unable to find menu: " + name);

      return *header.menu;
    }

    itemDef_s&
    find_item (menuDef_t& menu, const string& name)
    {
      for (int i (0); i < menu.itemCount; ++i)
      {
        itemDef_s* item (menu.items [i]);

        // The engine does not guarantee invariants. Validate pointers before
        // comparing names.
        //
        if ((item != nullptr) && (item->text != nullptr) && name == item->text)
          return *item;
      }

      throw runtime_error ("unable to find item: " + name);
    }

    MenuEventHandler*
    make_handler (const string& command)
    {
      MenuEventHandler* handler (new MenuEventHandler ());

      handler->eventType = 0;
      handler->eventData.unconditionalScript = _strdup (command.c_str ());

      return handler;
    }

    MenuEventHandlerSet*
    make_handler_set (const vector<string>& commands)
    {
      MenuEventHandlerSet* handler_set (new MenuEventHandlerSet ());

      handler_set->eventHandlerCount = static_cast<int>  (commands.size ());
      handler_set->eventHandlers = new MenuEventHandler* [commands.size ()];

      for (size_t i (0); i != commands.size (); ++i)
        handler_set->eventHandlers[i] = make_handler (commands[i]);

      return handler_set;
    }
  }

  menu::
  menu (scheduler& s)
  {
    // Menu instantiation comes first. Register the continuation in com_frame so
    // it runs only after the menu graph is actually there to work with.
    //
    s.post ("com_frame", [] ()
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

      // Action.
      //
      ([] (const string& menu_name, const string& item_name, const vector<string>& commands)
        {
          menuDef_t& menu (find_menu (menu_name));
          itemDef_s& item (find_item (menu, item_name));

          item.action = make_handler_set (commands);
        },

      // Expression.
      //
      [] (const string& menu_name, const string& item_name)
        {
          menuDef_t& menu (find_menu (menu_name));
          itemDef_s& item (find_item (menu, item_name));

          item.disabledExp = nullptr;
        });
    });
  }
}
