// Server/Zone/PartyContainer.cpp
// Per-party in-memory state -- 8 slots + leader index + loot rule + level
// range. The PartyFinderServer in WM mirrors a flattened view.
#include "../Shared/ShineTypes.h"
namespace fiesta { namespace {
struct PartyEntry { uint32 cid; uint8 uiSlot; bool bLeader; };
class PartyContainer { public: PartyEntry aEntries[8]; uint8 uiLootRule; };
}} // anonymous
