// Server/DataReader/SHN/MobKillLog.cpp
// Auto-generated: one-file-per-SHN split for MobKillLog.shn
#include "MobKillLog.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MobKillLogShn& MobKillLogShn::Get() { static MobKillLogShn s; return s; }

void MobKillLogShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobKillLog");
    if (!t) { SHINELOG_WARN("MobKillLog.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobKillLogRow rec;
        rec.uiMobID = ShnGetU32(*t, _r, "MobID");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobKillLog.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
