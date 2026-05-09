// Server/Zone/AdminSecLog.h
// 39 Prison/Admin / 40 Anti-cheat / 41 Logging.
// EVIDENCE: PDB_CONFIRMED  symbol: ShinePrison, PrisonServer, AmpersandCommand, AdminLvSet,
//                                  XTrap4Server, BadNameFilterData, BadNameFilter,
//                                  AccountLog, GameLogSession, EasyLogSys, MobHuntLog, ItemDropLog
#ifndef FIESTA_ZONE_ADMINSECLOG_H
#define FIESTA_ZONE_ADMINSECLOG_H
#include "ShineObject.h"
#include <string>
#include <vector>
#include <map>

namespace fiesta {

// 39
class ShinePrison    { public: static void Send (ShinePlayer* pk, uint32 uiSec); static void Release(ShinePlayer* pk); };
class PrisonServer   { public: static void Tick(); };
class AmpersandCommand { public: static bool Execute(ShinePlayer* pk, const std::string& rLine); };
class AdminLvSet     { public: static uint8 GetRequiredLv(const std::string& rCmd); };

// 40
class XTrap4Server { public:
    static bool Handshake(ShinePlayer* pk, const uint8* p, size_t n);
    static bool Validate (ShinePlayer* pk, const uint8* pToken, size_t n);
};
class BadNameFilterData { public:
    static bool IsBlocked(const std::string& rName);
    static void AddBlocked(const std::string& rName);
};

// 41
class EasyLogSys { public: static void Write(const char* szTopic, const char* szFmt, ...); };
class MobHuntLog { public: static void Record (CharID c, MobID s); };
class ItemDropLog{ public: static void Record (CharID c, ItemID i, uint16 q); };

} // namespace fiesta
#endif
