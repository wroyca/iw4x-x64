#pragma once

#include <libiw4x/types/xasset.hxx>

using  DB_FindXAssetHeader_t = XAssetHeader (*) (XAssetType type, const char* name);
extern DB_FindXAssetHeader_t DB_FindXAssetHeader;
