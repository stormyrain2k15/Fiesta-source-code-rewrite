// Server/DataReader/SHN/MobNoFadeIn.cpp
// Auto-generated: one-file-per-SHN split for MobNoFadeIn.shn
#include "MobNoFadeIn.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MobNoFadeInShn& MobNoFadeInShn::Get() { static MobNoFadeInShn s; return s; }

void MobNoFadeInShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobNoFadeIn");
    if (!t) { SHINELOG_WARN("MobNoFadeIn.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobNoFadeInRow rec;
        rec.kMobInx = ShnGetStr(*t, _r, "MobInx");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobNoFadeIn.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
