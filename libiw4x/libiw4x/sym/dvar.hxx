#pragma once

#include <cstdint>

#include <libiw4x/types/xasset.hxx>

using  Dvar_RegisterString_t = __int64 (*) (__int64, __int64, int, __int64);
extern Dvar_RegisterString_t Dvar_RegisterString;

using  Dvar_RegisterInt_t = __int64 (*) (__int64, int, int, int, int, __int64);
extern Dvar_RegisterInt_t Dvar_RegisterInt;

using  Dvar_FindVar_t = dvar_t* (*) (const char* name);
extern Dvar_FindVar_t Dvar_FindVar;

using  Dvar_SetString_t = __int64 (*) (__int64, __int64);
extern Dvar_SetString_t Dvar_SetString;

using  Dvar_SetFromStringByName_t = dvar_t* (*) (const char*, const char*);
extern Dvar_SetFromStringByName_t Dvar_SetFromStringByName;

using  Dvar_RegisterBool_t = __int64 (*) (__int64, __int64, int, __int64);
extern Dvar_RegisterBool_t Dvar_RegisterBool;
