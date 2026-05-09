// Server/Zone/ShineItemUnEquip.cpp
// Unequip an equipped item back into the inventory.
#include "Inventory.h"
#include "ShineObject.h"
namespace fiesta {
class ShineItemUnEquip {
public:
    static bool UnEquip(ShinePlayer* pkP, uint32 uiItemId);
};
bool ShineItemUnEquip::UnEquip(ShinePlayer* pkP, uint32 uiItemId) {
    if (!pkP) return false;
    return pkP->Inv().Unequip(uiItemId);
}
} // namespace fiesta
