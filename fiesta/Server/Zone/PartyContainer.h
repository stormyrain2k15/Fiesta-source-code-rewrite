// Server/Zone/PartyContainer.h
// In-memory party state -- 8 player slots, leader index, loot rule,
// level range. The PartyFinderServer in WM mirrors a flattened view via
// NC_INTER_PARTY_STATE_PUSH.
#ifndef SHINE_ZONE_PARTYCONTAINER_H
#define SHINE_ZONE_PARTYCONTAINER_H
#include "../Shared/ShineTypes.h"

namespace shine {

enum ePartyLootRule {
    PLR_FreeForAll       = 0,
    PLR_RoundRobin       = 1,
    PLR_LeaderOnly       = 2,
    PLR_NeedBeforeGreed  = 3
};

struct PartyEntry {
    uint32 cid;        // 0 == empty slot
    uint8  uiSlot;     // 0..7
    bool   bLeader;
    uint16 uiLevel;    // cached for level-range gates / loot scaling
    uint8  uiClass;
};

class PartyContainer {
public:
    enum { MAX_SLOTS = 8 };
    PartyEntry  aEntries[MAX_SLOTS];
    uint8       uiLootRule;        // ePartyLootRule
    uint16      uiMinLevel;
    uint16      uiMaxLevel;
    uint8       uiCount;           // populated slot count
    uint32      uiPartyId;

    PartyContainer();

    // Slot management. AddMember returns the assigned slot or 0xFF
    // on full party. RemoveMember compacts the array and re-points
    // the leader if necessary; returns true if the party is now empty.
    uint8 AddMember   (uint32 cid, uint16 lvl, uint8 cls);
    bool  RemoveMember(uint32 cid);
    bool  HasMember   (uint32 cid) const;
    uint32 LeaderCid  () const;
    void   SetLeader  (uint32 cid);
};

} // namespace shine
#endif
