// Server/Zone/PartyContainer.cpp
// Real implementation -- formerly a stub in an anonymous namespace.
#include "PartyContainer.h"

namespace fiesta {

PartyContainer::PartyContainer()
    : uiLootRule(PLR_FreeForAll), uiMinLevel(0), uiMaxLevel(0),
      uiCount(0), uiPartyId(0)
{
    for (int i = 0; i < MAX_SLOTS; ++i) {
        aEntries[i].cid     = 0;
        aEntries[i].uiSlot  = (uint8)i;
        aEntries[i].bLeader = false;
        aEntries[i].uiLevel = 0;
        aEntries[i].uiClass = 0;
    }
}

uint8 PartyContainer::AddMember(uint32 cid, uint16 lvl, uint8 cls) {
    for (int i = 0; i < MAX_SLOTS; ++i) {
        if (aEntries[i].cid == 0) {
            aEntries[i].cid     = cid;
            aEntries[i].uiLevel = lvl;
            aEntries[i].uiClass = cls;
            aEntries[i].bLeader = (uiCount == 0);
            ++uiCount;
            return (uint8)i;
        }
    }
    return 0xFF;
}

bool PartyContainer::RemoveMember(uint32 cid) {
    bool wasLeader = false;
    for (int i = 0; i < MAX_SLOTS; ++i) {
        if (aEntries[i].cid == cid) {
            wasLeader = aEntries[i].bLeader;
            aEntries[i].cid     = 0;
            aEntries[i].bLeader = false;
            aEntries[i].uiLevel = 0;
            aEntries[i].uiClass = 0;
            if (uiCount > 0) --uiCount;
            break;
        }
    }
    if (wasLeader) {
        // Reseat leader on the lowest-index occupied slot.
        for (int i = 0; i < MAX_SLOTS; ++i) {
            if (aEntries[i].cid != 0) {
                aEntries[i].bLeader = true;
                break;
            }
        }
    }
    return uiCount == 0;
}

bool PartyContainer::HasMember(uint32 cid) const {
    for (int i = 0; i < MAX_SLOTS; ++i)
        if (aEntries[i].cid == cid) return true;
    return false;
}

uint32 PartyContainer::LeaderCid() const {
    for (int i = 0; i < MAX_SLOTS; ++i)
        if (aEntries[i].bLeader) return aEntries[i].cid;
    return 0;
}

void PartyContainer::SetLeader(uint32 cid) {
    for (int i = 0; i < MAX_SLOTS; ++i) aEntries[i].bLeader = false;
    for (int i = 0; i < MAX_SLOTS; ++i) {
        if (aEntries[i].cid == cid) { aEntries[i].bLeader = true; break; }
    }
}

} // namespace fiesta
