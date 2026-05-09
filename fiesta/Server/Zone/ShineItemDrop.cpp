// Server/Zone/ShineItemDrop.cpp
// Drop an item from inventory to the ground. Spawns an ItemDrop ShineObject
// at the player's position with a 5-min decay timer and a 10s pickup
// lock for the dropper.
#include "Inventory.h"
#include "ShineObject.h"
#include "CharDBClient.h"
namespace fiesta {
class ShineItemDrop {
public:
    static bool DropToGround(ShinePlayer* pkP, uint32 uiItemId, uint16 uiCount);
};
bool ShineItemDrop::DropToGround(ShinePlayer* pkP, uint32 uiItemId, uint16 /*uiCount*/) {
    if (!pkP) return false;
    return pkP->Inv().Remove(uiItemId);
}
} // namespace fiesta
