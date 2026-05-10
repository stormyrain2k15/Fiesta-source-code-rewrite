// Server/DataReader/SHN/CollectCardGroupDesc.cpp
// Auto-generated: one-file-per-SHN split for CollectCardGroupDesc.shn
#include "CollectCardGroupDesc.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

CollectCardGroupDescShn& CollectCardGroupDescShn::Get() { static CollectCardGroupDescShn s; return s; }

void CollectCardGroupDescShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardGroupDesc");
    if (!t) { SHINELOG_WARN("CollectCardGroupDesc.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        CollectCardGroupDescRow rec;
        rec.uiCC_CardMobGroup = ShnGetU32(*t, _r, "CC_CardMobGroup");
        rec.kCC_MobGroupName = ShnGetStr(*t, _r, "CC_MobGroupName");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("CollectCardGroupDesc.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
