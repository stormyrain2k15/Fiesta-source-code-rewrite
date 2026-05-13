// Server/DataReader/SHN/DamageLvGapEVP.cpp
// Auto-generated: one-file-per-SHN split for DamageLvGapEVP.shn
#include "DamageLvGapEVP.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

DamageLvGapEVPShn& DamageLvGapEVPShn::Get() { static DamageLvGapEVPShn s; return s; }

void DamageLvGapEVPShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("DamageLvGapEVP");
    if (!t) { SHINELOG_WARN("DamageLvGapEVP.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        DamageLvGapEVPRow rec;
        rec.uiLvGap = (uint16)ShnGetU32(*t, _r, "LvGap");
        rec.uiDamageRate = (uint16)ShnGetU32(*t, _r, "DamageRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("DamageLvGapEVP.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
