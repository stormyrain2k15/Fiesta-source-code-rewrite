// Server/Zone/SoulStoneVendor.cpp
#include "SoulStoneVendor.h"
#include "ShineObject.h"
#include "ShineNPCTable.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

bool SoulStoneVendor::IsVendor(const std::string& rNpcMobName) {
    const ShineNPCRow* p = ShineNPCTable::Get().FindNPC(rNpcMobName);
    if (!p) return false;
    if (_stricmp(p->kRole.c_str(), "Merchant") != 0) return false;
    return _stricmp(p->kRoleArg0.c_str(), "SoulStone") == 0;
}

eSoulStoneBuyResult SoulStoneVendor::Buy(ShinePlayer*       pkP,
                                          const std::string& rNpcMobName,
                                          Inventory&         rInv,
                                          SoulStoneCounts&   rCounts,
                                          bool               bIsHp,
                                          uint16             uiQty) {
    if (!pkP) return SS_BUY_NOT_VENDOR;
    if (uiQty == 0) return SS_BUY_BAD_QTY;
    if (!IsVendor(rNpcMobName)) return SS_BUY_NOT_VENDOR;

    // Cap check (don't even debit Vis if we'd be wasting).
    uint16 cur = bIsHp ? rCounts.uiHpCount : rCounts.uiSpCount;
    if (cur >= kSoulStoneMaxCount) return SS_BUY_AT_CAP;
    if ((uint32)cur + (uint32)uiQty > (uint32)kSoulStoneMaxCount)
        uiQty = (uint16)(kSoulStoneMaxCount - cur);

    int32 unit  = SoulStoneSystem::PriceForLevel((int32)pkP->GetLevel());
    int64 total = (int64)unit * (int64)uiQty;
    if (rInv.Money() < total) return SS_BUY_NO_MONEY;

    rInv.AddMoney(-total);
    SoulStoneSystem::Grant(rCounts, bIsHp, uiQty);

    SHINELOG_INFO("SoulStoneVendor: %s sold %u %s soul stones to L%u for %lld Vis",
                  rNpcMobName.c_str(), (uint32)uiQty, bIsHp ? "HP" : "SP",
                  (uint32)pkP->GetLevel(), (long long)total);
    return SS_BUY_OK;
}

} // namespace fiesta
