// Server/Zone/GuildSystem.h
// 25 -- Guild / Storage / Academy / War / Tournament.
// EVIDENCE: PDB_CONFIRMED  symbol: GuildServer, GuildZone, GuildStorageManager,
//                                  GuildAcademy, GuildTournamentSystem, GuildWarManager
#ifndef FIESTA_ZONE_GUILDSYSTEM_H
#define FIESTA_ZONE_GUILDSYSTEM_H
#include "Inventory.h"
#include <map>
#include <vector>

namespace fiesta {

struct GuildMember { CharID c; uint8 rank; };
struct GuildRec {
    uint32 uiId;
    std::string kName;
    CharID uiMaster;
    std::vector<GuildMember> kMembers;
    int64 iGold;
    std::vector<ShineItem> kStorage;
    uint16 uiGrade;
    uint32 uiTournamentScore;
};

class GuildServer {
public:
    static GuildServer& Get();
    GuildRec* Create(const std::string& rName, CharID master);
    bool      Disband(uint32 uiGuildId, CharID requester);
    bool      Invite (uint32 uiGuildId, CharID member);
    bool      Leave  (CharID member);
    GuildRec* Find   (uint32 uiGuildId);
    GuildRec* FindByMember(CharID c);
private:
    std::map<uint32, GuildRec> m_kAll;
    uint32 m_uiNext;
    GuildServer() : m_uiNext(1) {}
};

class GuildZone           { public: static void OnPlayerLogin(ShinePlayer* pk); };
class GuildStorageManager {
public:
    static bool Put (uint32 uiGuildId, CharID c, const ShineItem& kIt);
    static bool Take(uint32 uiGuildId, CharID c, uint32 uiItemId);
};
class GuildAcademy        { public: static void GrantApprenticeReward(CharID master, CharID app); };

// Guild war declaration window. The original game only accepted declaration
// requests during a fixed weekday/hour band (so wars couldn't be declared
// at 4 AM on a Tuesday). Tunables editable in one place.
struct GuildWarWindow {
    uint8  uiDayMask;        // bit i = (1<<i) for Sunday..Saturday
    uint8  uiHourFrom;       // inclusive
    uint8  uiHourTo;         // exclusive
};
class GuildWarManager {
public:
    static bool Declare(uint32 uiAttacker, uint32 uiDefender);
    static void Tick();
    // Window control (default: Fri/Sat/Sun 19:00..23:00 local).
    static const GuildWarWindow& GetWindow();
    static void SetWindow(const GuildWarWindow& rWin);
    static bool InWindow(); // checks current localtime
};
class GuildTournamentSystem {
public:
    static void RegisterGuild(uint32 uiGuildId);
    static void Tick();
    static int32 LvGapMul(uint16 uiLvA, uint16 uiLvB);   // GuildTournamentLvGap
    static int32 OccupyPoints(uint16 uiSec);             // GuildTournamentOccupy
};

} // namespace fiesta
#endif
