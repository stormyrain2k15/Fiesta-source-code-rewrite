// Server/DataReader/SHN/AttendReward.cpp
// Auto-generated: one-file-per-SHN split for AttendReward.shn
#include "AttendReward.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

AttendRewardShn& AttendRewardShn::Get() { static AttendRewardShn s; return s; }

void AttendRewardShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("AttendReward");
    if (!t) { SHINELOG_WARN("AttendReward.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        AttendRewardRow rec;
        rec.uiAR_ID = (uint8)ShnGetU32(*t, _r, "AR_ID");
        rec.uiAR_Type = ShnGetU32(*t, _r, "AR_Type");
        rec.uiAR_Count = (uint8)ShnGetU32(*t, _r, "AR_Count");
        rec.kAR_ItemInx = ShnGetStr(*t, _r, "AR_ItemInx");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AttendReward.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
