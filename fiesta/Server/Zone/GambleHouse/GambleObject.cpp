// Server/Zone/GambleHouse/GambleObject.cpp
// In-world gamble apparatus (slot machine, dice table, card table).
// Each is a placed map object; players interact via NC_NPC_INTERACT.
#include "../ShineObject.h"
namespace shine {
struct GambleObject { uint32 uiObjId; uint16 uiMap; float x, y; uint8 uiKind; }; // 0=slot 1=dice 2=card
} // namespace shine
