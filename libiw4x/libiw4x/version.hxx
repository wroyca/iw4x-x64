#pragma once

// The numeric version format is AAAAABBBBBCCCCCDDDE where:
//
// AAAAA - major version number
// BBBBB - minor version number
// CCCCC - bugfix version number
// DDD   - alpha / beta (DDD + 500) version number
// E     - final (0) / snapshot (1)
//
// When DDDE is not 0, 1 is subtracted from AAAAABBBBBCCCCC. For example:
//
// Version      AAAAABBBBBCCCCCDDDE
//
// 0.1.0        0000000001000000000
// 0.1.2        0000000001000020000
// 1.2.3        0000100002000030000
// 2.2.0-a.1    0000200001999990010
// 3.0.0-b.2    0000299999999995020
// 2.2.0-a.1.z  0000200001999990011
//
#define LIBIW4X_VERSION       999990001ULL
#define LIBIW4X_VERSION_STR   "0.1.0-a.0.20251007171832"
#define LIBIW4X_VERSION_ID    "0.1.0-a.0.20251007171832"
#define LIBIW4X_VERSION_FULL  "0.1.0-a.0.20251007171832"

#define LIBIW4X_VERSION_MAJOR 0
#define LIBIW4X_VERSION_MINOR 1
#define LIBIW4X_VERSION_PATCH 0

#define LIBIW4X_PRE_RELEASE   true

#define LIBIW4X_SNAPSHOT_SN   20251007171832ULL
#define LIBIW4X_SNAPSHOT_ID   ""
