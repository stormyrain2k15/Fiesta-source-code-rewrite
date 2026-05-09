// Server/Zone/PartySlot.cpp
// Single slot view -- one player + their per-party stats (contribution,
// last-loot, ready-flag).
#include "../Shared/ShineTypes.h"
namespace fiesta {
struct PartySlot {
    uint32 cid;
    uint16 uiContribution;
    uint64 uiLastLootMs;
    uint8  uiFlags;     // bit0=ready bit1=in-zone bit2=offline
};
} // namespace fiesta
