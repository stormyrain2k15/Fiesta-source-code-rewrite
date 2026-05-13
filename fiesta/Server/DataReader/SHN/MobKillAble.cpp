// Server/DataReader/SHN/MobKillAble.cpp
// Auto-generated: one-file-per-SHN split for MobKillAble.shn
#include "MobKillAble.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MobKillAbleShn& MobKillAbleShn::Get() { static MobKillAbleShn s; return s; }

void MobKillAbleShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobKillAble");
    if (!t) { SHINELOG_WARN("MobKillAble.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobKillAbleRow rec;
        rec.uiMobKillInx = ShnGetU32(*t, _r, "MobKillInx");
        rec.uiCheat = (uint8)ShnGetU32(*t, _r, "Cheat");
        rec.uiMob = (uint8)ShnGetU32(*t, _r, "Mob");
        rec.uiUser = (uint8)ShnGetU32(*t, _r, "User");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobKillAble.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
