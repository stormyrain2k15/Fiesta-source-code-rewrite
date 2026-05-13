// Server/DataReader/SHN/MobConditionServer.cpp
// Auto-generated: one-file-per-SHN split for MobConditionServer.shn
#include "MobConditionServer.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MobConditionServerShn& MobConditionServerShn::Get() { static MobConditionServerShn s; return s; }

void MobConditionServerShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobConditionServer");
    if (!t) { SHINELOG_WARN("MobConditionServer.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobConditionServerRow rec;
        rec.kMobInx = ShnGetStr(*t, _r, "MobInx");
        rec.uiAniLv = (uint8)ShnGetU32(*t, _r, "AniLv");
        rec.uiMC_Type = ShnGetU32(*t, _r, "MC_Type");
        rec.uiMC_ValueMin = ShnGetU32(*t, _r, "MC_ValueMin");
        rec.uiMC_ValueMax = ShnGetU32(*t, _r, "MC_ValueMax");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobConditionServer.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
