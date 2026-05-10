// Server/Zone/GambleHouse/GBAuxTables.h
// FEATURE: casino -- four ancillary single-purpose tables. Each is a
// thin per-SHN binder; one shared header keeps the slot/dice
// orchestrator's include list manageable.
#ifndef FIESTA_ZONE_GAMBLEHOUSE_GBAUXTABLES_H
#define FIESTA_ZONE_GAMBLEHOUSE_GBAUXTABLES_H
#include "../../../Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace fiesta {

// ----- GBReward (3 cols: GameType, RewardType, Item_INX) -----
struct LegacyGBRewardRow {
    uint32      uiGameType;
    uint32      uiRewardType;
    std::string kItemInx;
};
class GBRewardTable {
public:
    static GBRewardTable& Get();
    bool   Bind();
    void   FindByGameType(uint32 uiGameType,
                          std::vector<LegacyGBRewardRow>& rOut) const;
private:
    GBRewardTable() {}
    std::vector<LegacyGBRewardRow> m_kRows;
};

// ----- GBBanTime (3 cols: GameType, GB_Ban, GB_BanTime) -----
class GBBanTimeTable {
public:
    static GBBanTimeTable& Get();
    bool   Bind();
    uint32 BanMs(uint32 uiGameType) const;     // 0 = no ban
private:
    GBBanTimeTable() {}
    std::map<uint32, uint32> m_kRows;          // game -> ms
};

// ----- GBJoinGameMember (3 cols: GameType, MinJoinMember, MaxJoinMember) -----
struct GBJoinRow { uint8 uiMin; uint8 uiMax; };
class GBJoinGameMemberTable {
public:
    static GBJoinGameMemberTable& Get();
    bool   Bind();
    bool   Find(uint32 uiGameType, GBJoinRow& rOut) const;
private:
    GBJoinGameMemberTable() {}
    std::map<uint32, GBJoinRow> m_kRows;
};

// ----- GBEventCode (3 cols: GameType, GB_ANI, GB_ECode) -----
class GBEventCodeTable {
public:
    static GBEventCodeTable& Get();
    bool   Bind();
    int32  Code(uint32 uiGameType, uint32 uiAniIdx) const;
private:
    GBEventCodeTable() {}
    // (gameType*16 + ani) -> ecode
    std::map<uint64, int32> m_kRows;
};

} // namespace fiesta
#endif
