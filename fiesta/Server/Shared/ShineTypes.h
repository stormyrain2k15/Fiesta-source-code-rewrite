// Server/Shared/ShineTypes.h
// common typedefs and build switches.
#ifndef SHINE_SHINE_TYPES_H
#define SHINE_SHINE_TYPES_H

#if !defined(_WIN32)
#  error "Shine server/client targets Win32 only (VS2010 / v100 toolset)."
#endif

// VS2010 compat: do not require C++11 features.
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <map>

namespace shine {

typedef signed   char      int8;
typedef unsigned char      uint8;
typedef signed   short     int16;
typedef unsigned short     uint16;
typedef signed   int       int32;
typedef unsigned int       uint32;
typedef signed   __int64   int64;
typedef unsigned __int64   uint64;

typedef uint32 Handle;            // generic ShineObject handle id
typedef uint32 CharID;
typedef uint32 AccountID;
typedef uint32 MobID;
typedef uint16 MapID;
typedef uint16 ItemID;
typedef uint32 SkillID;
typedef uint16 NCOpcode;          // NETCOMMAND opcode (16-bit, network order on wire)

const Handle  INVALID_HANDLE = 0;
const CharID  INVALID_CHARID = 0;

} // namespace shine

#endif
