// Server/Zone/EquipSummaryBuilder.cpp
#include "EquipSummaryBuilder.h"
#include "GroupTables.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/ITableBase.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

void AddItemContribution(EQUIPSUMMARY* pOut, const ShineItem& kItem) {
    if (!pOut) return;
    if (!kItem.bEquipped) return;
    const ItemInfoRow* p = ITableBase<ItemInfoRow>::ms_pkTable
        ? ITableBase<ItemInfoRow>::ms_pkTable->Find(kItem.uiInxName) : NULL;
    if (!p) return;

    // Average min/max -- the formula uses random in [Min..Max] at hit time;
    // the equip summary stores the midpoint so the BATTLESTAT shows the
    // expected value. Variance is then re-applied in CalcDamage.
    int32 wc = (int32)((p->MinWC + p->MaxWC) / 2);
    int32 ma = (int32)((p->MinMA + p->MaxMA) / 2);

    pOut->nATK     += wc;
    pOut->nMATK    += ma;
    pOut->nDEF     += (int32)p->AC;
    pOut->nMDEF    += (int32)p->MR;
    pOut->nHP      += (int32)p->MaxHP;
    pOut->nSP      += (int32)p->MaxSP;
    pOut->nCritical+= (int32)p->CriRate;
    pOut->nAccuracy+= (int32)p->HitRatePlus;
    pOut->nDodge   += (int32)p->EvaRatePlus;

    // Upgrade-level bonus -- BasicUpInx baseline + AddUpInx per +level.
    if (kItem.uiEnchant > 0 && p->BasicUpInx > 0) {
        int32 upATK = (int32)p->BasicUpInx * (int32)kItem.uiEnchant;
        if (kItem.uiEnchant > 1 && p->AddUpInx > 0)
            upATK += (int32)p->AddUpInx * ((int32)kItem.uiEnchant - 1);
        if (p->WeaponType > 0)        pOut->nUpgradeATK += upATK;
        else if (p->ArmorType > 0 || p->ShieldAC > 0) pOut->nUpgradeDEF += upATK;
    }

    if (p->ShieldAC > 0)
        pOut->nDEF += (int32)p->ShieldAC;

    // GradeItemOption.shn (20-col wide row) -- per-item-grade stat boost.
    // Keyed by ItemIndex; if the item's row has Grade>0 we sum every
    // column on the matching grade row into the summary.
    const GradeItemOptionRow* pGrade = GradeRandomTables::Get().FindGrade(kItem.uiInxName);
    if (pGrade) {
        // Stat-side bonuses go through the equip summary; pure-stat (STR..MEN)
        // are returned as additive raw stat which is folded by BuildBattleStat
        // through the BUFFMODIFIERS path. For now, we surface the equip-level
        // ones directly.
        pOut->nHP        += (int32)pGrade->uiMaxHP;
        pOut->nSP        += (int32)pGrade->uiMaxSP;
        pOut->nCritical  += (int32)pGrade->uiCritical;
        pOut->nAccuracy  += (int32)pGrade->uiToHitPlus;
        pOut->nBlock     += (int32)pGrade->uiToBlockPlus;
        pOut->nMoveSpeed += (int32)pGrade->uiMoveSpdRate;
        pOut->nATK       += (int32)pGrade->uiAbsoluteAttack;
        // ResistPoison / Deaseas / Curse / MoveSpdDown route to abstate
        // resists; ToHitRate / ToBlockRate are permille rates folded by
        // BuildBattleStat. The remaining STR/CON/DEX/INT/MEN are picked
        // up via RAWCHARSTAT additive in BuildBattleStat.
    }
}

void BuildEquipSummary(EQUIPSUMMARY* pOut, const Inventory& kInv) {
    if (!pOut) return;
    pOut->Clear();
    const std::vector<ShineItem>& v = kInv.All();
    for (size_t i = 0; i < v.size(); ++i)
        AddItemContribution(pOut, v[i]);
}

} // namespace fiesta
