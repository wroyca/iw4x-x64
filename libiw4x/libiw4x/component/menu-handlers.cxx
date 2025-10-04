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
}
