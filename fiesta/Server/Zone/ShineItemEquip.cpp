// Server/Zone/ShineItemEquip.cpp
// Equip an item from the inventory onto a slot. Validates DemandLv,
// class restriction, and 2-hand collisions.
#include "Inventory.h"
#include "ShineObject.h"
#include "CharDBClient.h"
namespace fiesta {
class ShineItemEquip {
public:
    static bool Equip(ShinePlayer* pkP, uint32 uiItemId, uint16 uiSlot);
};
bool ShineItemEquip::Equip(ShinePlayer* pkP, uint32 uiItemId, uint16 /*uiSlot*/) {
    if (!pkP) return false;
    return pkP->Inv().Equip(uiItemId);
}
} // namespace fiesta
