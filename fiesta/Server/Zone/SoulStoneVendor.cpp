// Server/Zone/SoulStoneVendor.cpp
#include "SoulStoneVendor.h"
#include "ShineObject.h"
#include "ShineNPCTable.h"
#include "ClassParamTable.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

bool SoulStoneVendor::IsVendor(const std::string& rNpcMobName) {
    const ShineNPCRow* p = ShineNPCTable::Get().FindNPC(rNpcMobName);
    if (!p) return false;
    if (_stricmp(p->kRole.c_str(), "Merchant") != 0) return false;
    return _stricmp(p->kRoleArg0.c_str(), "SoulStone") == 0;
}

// Forward decl mirror of SoulStoneSystem.cpp -- placeholder until
// ShinePlayerCharacter exposes the class field.
static eShineClass ClassOf(ShinePlayer* /*pk*/) { return SC_FIGHTER; }

eSoulStoneBuyResult SoulStoneVendor::Buy(ShinePlayer*       pkP,
                                          const std::string& rNpcMobName,
                                          Inventory&         rInv,
                                          SoulStoneCounts&   rCounts,
                                          bool               bIsHp,
                                          uint16             uiQty) {
    if (!pkP) return SS_BUY_NOT_VENDOR;
    if (uiQty == 0) return SS_BUY_BAD_QTY;
    if (!IsVendor(rNpcMobName)) return SS_BUY_NOT_VENDOR;

    eShineClass eC = ClassOf(pkP);
    uint16 uiL = pkP->GetLevel();

    // Cap.
    int32 cap = bIsHp ? ClassParamTable::Get().SoulHpCapForLevel(eC, uiL)
                      : ClassParamTable::Get().SoulSpCapForLevel(eC, uiL);
    if (cap <= 0) cap = 9999;
    uint16 cur = bIsHp ? rCounts.uiHpCount : rCounts.uiSpCount;
    if (cur >= (uint16)cap) return SS_BUY_AT_CAP;
    if ((int32)cur + (int32)uiQty > cap) uiQty = (uint16)(cap - cur);

    // Price.
    int32 unit = bIsHp ? ClassParamTable::Get().SoulHpPriceForLevel(eC, uiL)
                       : ClassParamTable::Get().SoulSpPriceForLevel(eC, uiL);
    if (unit <= 0) return SS_BUY_NOT_VENDOR;       // no price row -> not for sale
    int64 total = (int64)unit * (int64)uiQty;
    if (rInv.Money() < total) return SS_BUY_NO_MONEY;

    rInv.AddMoney(-total);
    SoulStoneSystem::Grant(rCounts, eC, uiL, bIsHp, uiQty);

    SHINELOG_INFO("SoulStoneVendor: %s sold %u %s soul stones to L%u for %lld Vis (%d/ea)",
                  rNpcMobName.c_str(), (uint32)uiQty, bIsHp ? "HP" : "SP",
                  (uint32)uiL, (long long)total, (int)unit);
    return SS_BUY_OK;
}

} // namespace fiesta
