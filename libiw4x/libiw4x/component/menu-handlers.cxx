#include <libiw4x/component/menu-handlers.hxx>

namespace iw4x
{
  menu_handlers::menu_handlers () = default;

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
