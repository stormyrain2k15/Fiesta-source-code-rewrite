// Server/Zone/ShineItemSell.cpp
// NPC shop sell. Sell-back at 25% of base price. Adds the item to the
// repurchase buffer (ShineItemRepurchase.cpp) so the player can buy
// back within the same session.
#include "Inventory.h"
#include "CharDBClient.h"
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"
namespace shine {
class ShineItemSell {
public:
    // Sell by item-id (not slot) -- routes through the existing
    // Inventory::Remove path. Slot-targeted sells are layered on top.
    static bool Sell(ShinePlayer* pkP, uint32 uiItemId, uint16 uiCount, uint32 uiUnitPrice);
};
bool ShineItemSell::Sell(ShinePlayer* pkP, uint32 uiItemId, uint16 uiCount, uint32 uiUnitPrice) {
    if (!pkP || uiCount == 0) return false;
    int64 gain = (int64)uiUnitPrice * (int64)uiCount * 25 / 100;
    if (!pkP->Inv().Remove(uiItemId)) return false;
    pkP->AddMoney(gain);
    SHINELOG_INFO("Sell cid=%u item=%u x%u gain=%lld",
                  pkP->GetCharID(), uiItemId, (uint32)uiCount, (long long)gain);
    return true;
}
} // namespace shine
