#pragma once

#include <libiw4x/database.hxx>

namespace iw4x
{
  using  Com_Frame_Try_Block_Function_t = void (*) ();
  extern Com_Frame_Try_Block_Function_t Com_Frame_Try_Block_Function;

  using  DB_FindXAssetHeader_t = XAssetHeader (*) (XAssetType type, const char* name);
  extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

  using  Dvar_RegisterString_t = __int64 (*) (__int64, __int64, int, __int64);
  extern Dvar_RegisterString_t Dvar_RegisterString;

  using  Dvar_RegisterInt_t = __int64 (*) (__int64, int, int, int, int, __int64);
  extern Dvar_RegisterInt_t Dvar_RegisterInt;

  using  Dvar_FindVar_t = dvar_t* (*) (const char* name);
  extern Dvar_FindVar_t Dvar_FindVar;

  using  Dvar_SetString_t = __int64 (*) (__int64, __int64);
  extern Dvar_SetString_t Dvar_SetString;

  using  Dvar_SetFromStringByName_t = __int64 (*) (__int64, __int64);
  extern Dvar_SetFromStringByName_t Dvar_SetFromStringByName;

  using  Dvar_RegisterBool_t = __int64 (*) (__int64, __int64, int, __int64);
  extern Dvar_RegisterBool_t Dvar_RegisterBool;

  using  NET_Config_t = int (*) (int);
  extern NET_Config_t NET_Config;

  using  NET_GetDvars_t = bool (*) ();
  extern NET_GetDvars_t NET_GetDvars;

  using  NET_SendPacket_t = bool (*) (size_t, char*, int*);
  extern NET_SendPacket_t NET_SendPacket;
}
