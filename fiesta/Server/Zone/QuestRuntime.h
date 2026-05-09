// Server/Zone/QuestRuntime.h
// 17 -- sidecar that drives quest lifecycle on top of QuestTable
//        (Header / Reward / Looting / Hunting / Produce / Meeting).
//
// Per character we keep a tiny ledger of active quests and their progress
// counters. The runtime exposes:
//
//   StartQuest(player, handle, npc)    -- validates level + start NPC.
//   OnMobKill (player, mobName)        -- bumps Hunting kill counts.
//   OnItemPick(player, itemName)       -- bumps Looting drop counts.
//   OnNpcMeet (player, npcName)        -- ticks Meeting / talks-to quests.
//   OnProduce (player, itemMade)       -- ticks Produce craft-it counters.
//   CompleteQuest(player, handle, npc) -- validates + grants Reward rows.
//   GiveUp     (player, handle)        -- removes from active set.
//
// Reward delivery currently logs the grant intent + adds Vis when
// Type=="MoneyExp"; full item-grant requires the Inventory item-name
// resolver to be wired which is the next pass.
//
// EVIDENCE: PDB_CONFIRMED  symbol: cQuest, QuestStart, QuestComplete.
#ifndef FIESTA_ZONE_QUESTRUNTIME_H
#define FIESTA_ZONE_QUESTRUNTIME_H
#include "WorldTables.h"
#include <map>
#include <vector>
#include <string>

namespace fiesta {

class ShinePlayer;
class Inventory;

struct QuestProgress {
    uint16 uiHandle;
    uint64 uiStartedAtMs;
    std::map<std::string, uint16> kHuntCount;     // mob -> kills so far
    std::map<std::string, uint16> kLootCount;     // item -> picked so far
    std::map<std::string, uint16> kMetCount;      // npc -> times met
    std::map<std::string, uint16> kProduceCount;  // craftedItem -> count
};

enum eQuestResult {
    QR_OK                 = 0,
    QR_UNKNOWN_HANDLE     = 1,
    QR_LEVEL_TOO_LOW      = 2,
    QR_LEVEL_TOO_HIGH     = 3,
    QR_WRONG_NPC          = 4,
    QR_ALREADY_ACTIVE     = 5,
    QR_NOT_ACTIVE         = 6,
    QR_GOALS_INCOMPLETE   = 7,
    QR_TIMED_OUT          = 8
};

class QuestRuntime {
public:
    // Per-player state container. Lives on the ShinePlayer record.
    struct PlayerLog {
        std::map<uint16, QuestProgress> kActive;
    };

    static eQuestResult StartQuest   (ShinePlayer* pkP, PlayerLog& rL, uint16 uiHandle, const std::string& rNpc);
    static eQuestResult CompleteQuest(ShinePlayer* pkP, PlayerLog& rL, Inventory& rInv,
                                       uint16 uiHandle, const std::string& rNpc);
    static eQuestResult GiveUp       (PlayerLog& rL, uint16 uiHandle);

    static void OnMobKill (PlayerLog& rL, const std::string& rMobName);
    static void OnItemPick(PlayerLog& rL, const std::string& rItemName);
    static void OnNpcMeet (PlayerLog& rL, const std::string& rNpcName);
    static void OnProduce (PlayerLog& rL, const std::string& rItemMade);

    // Returns true if every Hunting / Looting / Meeting / Produce goal is met.
    static bool AllGoalsComplete(const PlayerLog& rL, uint16 uiHandle);
};

} // namespace fiesta
#endif
