#pragma once

#include <libiw4x/types/net.hxx>

using  CL_ConnectFromParty_t = void (*) (int, void*, netadr_t, int, int, const char*, const char*);
extern CL_ConnectFromParty_t CL_ConnectFromParty;
