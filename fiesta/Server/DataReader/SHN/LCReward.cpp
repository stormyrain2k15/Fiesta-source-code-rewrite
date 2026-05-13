// Server/DataReader/SHN/LCReward.cpp
// Auto-generated: one-file-per-SHN split for LCReward.shn
#include "LCReward.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

LCRewardShn& LCRewardShn::Get() { static LCRewardShn s; return s; }

void LCRewardShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("LCReward");
    if (!t) { SHINELOG_WARN("LCReward.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        LCRewardRow rec;
        rec.uiLCR_Group = (uint8)ShnGetU32(*t, _r, "LCR_Group");
        rec.kItem_Inx = ShnGetStr(*t, _r, "Item_Inx");
        rec.uiLCR_Lot = (uint8)ShnGetU32(*t, _r, "LCR_Lot");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("LCReward.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
