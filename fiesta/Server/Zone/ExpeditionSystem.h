// Server/Zone/ExpeditionSystem.h
// 24+ -- Expedition (raid of parties).
//
// EVIDENCE: PDB_CONFIRMED  symbol: ExpeditionPartyWin (client),
//                                  ExpeditionMemberAddWin,
//                                  ExpeditionLootSelectWin
// EVIDENCE: SOURCE_HINT    file: docs/spec_pack/function_names/full_function_symbols_filtered.csv
//                                  ("Party / Raid / Party Finder")
//
// Conceptually an Expedition is *a party of parties*: up to 6 sub-parties,
// each a normal Party from `Party.h`. Membership is by Party UID, not
// CharID, so when a party leader joins an expedition every member of that
// party rides along.
//
// Loot modes: FREE / MASTER / TURN.  In MASTER mode the expedition leader
// distributes drops; in TURN mode the engine round-robins per drop.
//
// Buffs: a master-buff applies to every sub-party while the expedition is
// active; this matches the GuildTournamentMasterBuff data the original
// game used during Guild Tournament expeditions.
#ifndef FIESTA_ZONE_EXPEDITIONSYSTEM_H
#define FIESTA_ZONE_EXPEDITIONSYSTEM_H
#include "Party.h"
#include <vector>
#include <map>

namespace fiesta {

enum eExpeditionLoot {
    EL_FREE   = 0,
    EL_MASTER = 1,
    EL_TURN   = 2
};

struct ExpeditionRec {
    uint32                  uiExpId;
    CharID                  uiLeader;            // leader of expedition (a party leader)
    std::vector<uint32>     kPartyIds;           // up to 6 PartyContainer::uiPartyId
    eExpeditionLoot         eLoot;
    uint32                  uiTurnCursor;        // index into flattened member list
    uint32                  uiMasterBuffID;      // 0 if disabled
};

class ExpeditionSystem {
public:
    static ExpeditionSystem& Get();

    // The leader's party becomes the founding sub-party.
    ExpeditionRec*  Create(CharID uiLeader, eExpeditionLoot eLoot);
    bool            JoinParty (uint32 uiExpId, uint32 uiPartyId);
    bool            LeaveParty(uint32 uiExpId, uint32 uiPartyId);
    bool            Disband   (uint32 uiExpId, CharID uiRequester);
    ExpeditionRec*  Find      (uint32 uiExpId);
    ExpeditionRec*  FindByMember(CharID c);

    // Loot routing. Returns CharID who receives the drop based on mode.
    // Returns INVALID_CHARID if expedition not found.
    CharID          RouteLoot(uint32 uiExpId);

    // Apply / refresh master buff to every member.
    void            ApplyMasterBuff(uint32 uiExpId, uint32 uiBuffID);
    // Tick: re-apply buffs every N seconds, prune stale parties.
    void            Tick();
private:
    ExpeditionSystem() : m_uiNext(1) {}
    std::map<uint32, ExpeditionRec> m_kAll;
    std::map<uint32, uint32>        m_kPartyIdx;     // partyId -> expId
    uint32                          m_uiNext;
};

} // namespace fiesta
#endif
