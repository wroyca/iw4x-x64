#pragma once

#include <cstdint>

#include <libiw4x/types/net.hxx>

using  NET_Config_t = int (*) (int);
extern NET_Config_t NET_Config;

using  NET_GetDvars_t = bool (*) ();
extern NET_GetDvars_t NET_GetDvars;

using  NET_SendPacket_t = bool (*) (size_t, char*, int*);
extern NET_SendPacket_t NET_SendPacket;

using  NET_StringToAdr_t = bool (*) (const char*, netadr_t*);
extern NET_StringToAdr_t NET_StringToAdr;

using  NET_OutOfBandPrint_t = void (*) (int, netadr_t*, const char*, ...);
extern NET_OutOfBandPrint_t NET_OutOfBandPrint;
