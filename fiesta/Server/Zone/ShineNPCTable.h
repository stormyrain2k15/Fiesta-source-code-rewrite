// Server/Zone/ShineNPCTable.h
// typed loader for World/NPC.txt (ShineNPC + LinkTable tables).
// Source format: TableScript (#Table ShineNPC ; #Table LinkTable ; #recordin ...).
#ifndef FIESTA_ZONE_SHINENPCTABLE_H
#define FIESTA_ZONE_SHINENPCTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ShineNPCRow {
    std::string kMobName;       // unique key (NPC index name)
    std::string kMap;           // map id (e.g. "RouN")
    int32       iCoordX, iCoordY;
    int16       iDirect;
    uint8       uiNPCMenu;      // 1 = ClientMenu / Merchant / etc.
    std::string kRole;          // "Merchant", "QuestNpc", "ClientMenu", ...
    std::string kRoleArg0;      // sub-role argument (Weapon, SoulStone, Item, ...)
};

struct LinkRow {
    std::string kArgument;
    std::string kMapServer;
    std::string kMapClient;
    int32       iCoordX, iCoordY;
    int16       iDirect;
    uint8       uiParty;
};

class ShineNPCTable {
public:
    static ShineNPCTable& Get();
    bool   Load(const std::string& rRoot);
    // Walk every loaded row, create a ShineNPC, register it with NPCManager,
    // and (if the row's Map resolves) add it to the matching Field. Returns
    // the number of NPCs actually spawned.
    size_t SpawnAll();
    size_t NPCCount () const { return m_kNPCs .size(); }
    size_t LinkCount() const { return m_kLinks.size(); }
    const ShineNPCRow* FindNPC(const std::string& rMobName) const;
    const std::vector<ShineNPCRow>& NPCs () const { return m_kNPCs ; }
    const std::vector<LinkRow>&     Links() const { return m_kLinks; }
private:
    ShineNPCTable() {}
    std::vector<ShineNPCRow>      m_kNPCs;
    std::vector<LinkRow>          m_kLinks;
    std::map<std::string, size_t> m_kIndex;
};

} // namespace fiesta
#endif
