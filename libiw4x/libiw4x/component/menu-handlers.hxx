#pragma once

#include <libiw4x/database.hxx>
#include <libiw4x/symbols.hxx>

#include <libiw4x/utility-win32.hxx>

namespace iw4x
{
  inline XAssetEntry* g_assetEntryPool = reinterpret_cast<XAssetEntry*> (0x141567FD0);
  inline unsigned short* db_hashTable = reinterpret_cast<unsigned short*> (0x141551E90);
  inline XAssetHeader (*DB_FindXAssetHeader) (XAssetType type, const char* name) = reinterpret_cast<XAssetHeader (*) (XAssetType, const char*)> (0x140129220);

  class menu_handlers
  {
  public:
    menu_handlers ();

    menuDef_t*
    find_menu (const string& name);

    itemDef_s*
    find_item (menuDef_t*, const string& name);
  };
}
