// Server/DataReader/SHN/MobLifeTime.cpp
// Auto-generated: one-file-per-SHN split for MobLifeTime.shn
#include "MobLifeTime.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MobLifeTimeShn& MobLifeTimeShn::Get() { static MobLifeTimeShn s; return s; }

void MobLifeTimeShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobLifeTime");
    if (!t) { SHINELOG_WARN("MobLifeTime.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobLifeTimeRow rec;
        rec.kMobIndex = ShnGetStr(*t, _r, "MobIndex");
        rec.uiLifeTime = (uint16)ShnGetU32(*t, _r, "LifeTime");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobLifeTime.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
