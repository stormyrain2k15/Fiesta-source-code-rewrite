// Server/DataReader/SHN/MoverUpgradeEffect.cpp
// Auto-generated: one-file-per-SHN split for MoverUpgradeEffect.shn
#include "MoverUpgradeEffect.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MoverUpgradeEffectShn& MoverUpgradeEffectShn::Get() { static MoverUpgradeEffectShn s; return s; }

void MoverUpgradeEffectShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MoverUpgradeEffect");
    if (!t) { SHINELOG_WARN("MoverUpgradeEffect.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MoverUpgradeEffectRow rec;
        rec.uiRunSpeed = (uint16)ShnGetU32(*t, _r, "RunSpeed");
        rec.uiHPSPRecoveryTick = (uint16)ShnGetU32(*t, _r, "HPSPRecoveryTick");
        rec.uiHPSPRecovery = (uint16)ShnGetU32(*t, _r, "HPSPRecovery");
        rec.uiCastingTime = (uint16)ShnGetU32(*t, _r, "CastingTime");
        rec.uiCastingCoolTime = (uint16)ShnGetU32(*t, _r, "CastingCoolTime");
        rec.kEffectFileName = ShnGetStr(*t, _r, "EffectFileName");
        rec.kAbStateIDX = ShnGetStr(*t, _r, "AbStateIDX");
        rec.uiStrength = (uint8)ShnGetU32(*t, _r, "Strength");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MoverUpgradeEffect.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
