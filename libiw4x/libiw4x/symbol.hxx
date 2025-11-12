#pragma once

#include <libiw4x/database.hxx>

namespace iw4x
{
  using  Com_Frame_Try_Block_Function_t = void (*) ();
  extern Com_Frame_Try_Block_Function_t Com_Frame_Try_Block_Function;

  using  DB_FindXAssetHeader_t = XAssetHeader (*) (XAssetType type, const char* name);
  extern DB_FindXAssetHeader_t DB_FindXAssetHeader;
}
