#pragma once

#include <libiw4x/utility/utility-win32.hxx>

// 29
//
enum netsrc_t
{
  NS_CLIENT1 = 0x0,
  NS_SERVER = 0x1,
  NS_MAXCLIENTS = 0x1,
  NS_PACKET = 0x2,
};

// 100
//
enum netadrtype_t
{
  NA_BOT = 0x0,
  NA_BAD = 0x1,
  NA_LOOPBACK = 0x2,
  NA_BROADCAST = 0x3,
  NA_IP = 0x4,
  NA_IPX = 0x5,
  NA_BROADCAST_IPX = 0x6,
};

// 1015
struct XNADDR
{
  in_addr ina;
  in_addr inaOnline;
  unsigned short wPort;
  unsigned short wPortOnline;
  char abEnet[4];
  char abOnline[20];
};

// 1315
//
struct netadr_t
{
  netadrtype_t type;
  char ip[4];
  unsigned short port;
  char ipx[10];
};

// 1362
struct XNKID
{
  char ab[8];
};

// 1363
struct XNKEY
{
  char ab[16];
};

// 1364
struct XSESSION_INFO
{
  XNKID sessionID;
  XNADDR hostAddress;
  XNKEY keyExchangeKey;
};

// 1474
//
struct msg_t
{
  int overflowed;
  int readOnly;
  char *data;
  char *splitData;
  int maxsize;
  int cursize;
  int splitSize;
  int readcount;
  int bit;
  int lastEntityRef;
};
