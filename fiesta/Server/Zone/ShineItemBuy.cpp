// Server/Zone/ShineItemBuy.cpp
// NPC shop purchase. Handler in ZoneHandlers.cpp / NPCSystem.cpp drives
// here; this file owns the price * count + money-debit + Item_Create.
#include "Inventory.h"
#include "CharDBClient.h"
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"
namespace shine {
class ShineItemBuy {
public:
    static bool Buy(ShinePlayer* pkP, uint32 uiNpcID, uint32 uiItemId,
                    uint16 uiCount, uint32 uiUnitPrice);
};
bool ShineItemBuy::Buy(ShinePlayer* pkP, uint32 uiNpcID, uint32 uiItemId,
                       uint16 uiCount, uint32 uiUnitPrice) {
    if (!pkP || uiCount == 0) return false;
    int64 total = (int64)uiUnitPrice * (int64)uiCount;
    if ((int64)pkP->GetMoney() < total) return false;
    pkP->AddMoney(-total);
    ShineItem it; memset(&it, 0, sizeof(it));
    it.uiItemId = uiItemId;  it.uiInxName = uiItemId; it.uiCount = uiCount;
    pkP->Inv().Add(it);
    CharDBClient::Get().ItemCreate(pkP->GetCharID(), uiItemId, uiCount, 0);
    SHINELOG_INFO("Buy cid=%u npc=%u item=%u x%u price=%lld",
                  pkP->GetCharID(), uiNpcID, uiItemId, (uint32)uiCount, (long long)total);
    return true;
}
} // namespace shine
