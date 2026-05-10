// Server/DataReader/SHN/MobRegenAni.cpp
// Auto-generated: one-file-per-SHN split for MobRegenAni.shn
#include "MobRegenAni.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MobRegenAniShn& MobRegenAniShn::Get() { static MobRegenAniShn s; return s; }

void MobRegenAniShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobRegenAni");
    if (!t) { SHINELOG_WARN("MobRegenAni.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobRegenAniRow rec;
        rec.kMobIDX = ShnGetStr(*t, _r, "MobIDX");
        rec.uiRegenTime = (uint16)ShnGetU32(*t, _r, "RegenTime");
        rec.kGroupAbStateIDX = ShnGetStr(*t, _r, "GroupAbStateIDX");
        rec.uiIsAggro = (uint8)ShnGetU32(*t, _r, "IsAggro");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobRegenAni.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
