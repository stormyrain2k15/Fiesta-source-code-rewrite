// Server/DataReader/SHN/ShineReward.cpp
// Auto-generated: one-file-per-SHN split for ShineReward.shn
#include "ShineReward.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ShineRewardShn& ShineRewardShn::Get() { static ShineRewardShn s; return s; }

void ShineRewardShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ShineReward");
    if (!t) { SHINELOG_WARN("ShineReward.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ShineRewardRow rec;
        rec.uiRewardHandle = (uint16)ShnGetU32(*t, _r, "RewardHandle");
        rec.uiRewardType = (uint8)ShnGetU32(*t, _r, "RewardType");
        rec.kArgument = ShnGetStr(*t, _r, "Argument");
        rec.uiQuantity = ShnGetU32(*t, _r, "Quantity");
        rec.iUpgrade = (int16)ShnGetI32(*t, _r, "Upgrade");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = (int16)ShnGetI32(*t, _r, "UnkCol6");
        rec.iUnkCol7 = (int16)ShnGetI32(*t, _r, "UnkCol7");
        rec.iUnkCol8 = (int16)ShnGetI32(*t, _r, "UnkCol8");
        rec.iUnkCol9 = (int16)ShnGetI32(*t, _r, "UnkCol9");
        rec.iUnkCol10 = (int16)ShnGetI32(*t, _r, "UnkCol10");
        rec.iUnkCol11 = (int16)ShnGetI32(*t, _r, "UnkCol11");
        rec.iUnkCol12 = (int16)ShnGetI32(*t, _r, "UnkCol12");
        rec.iUnkCol13 = (int16)ShnGetI32(*t, _r, "UnkCol13");
        rec.uiOptionDegree = (uint16)ShnGetU32(*t, _r, "OptionDegree");
        rec.uiTitleDegree = ShnGetU32(*t, _r, "TitleDegree");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ShineReward.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
