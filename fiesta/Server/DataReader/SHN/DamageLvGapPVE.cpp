// Server/DataReader/SHN/DamageLvGapPVE.cpp
// Auto-generated: one-file-per-SHN split for DamageLvGapPVE.shn
#include "DamageLvGapPVE.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

DamageLvGapPVEShn& DamageLvGapPVEShn::Get() { static DamageLvGapPVEShn s; return s; }

void DamageLvGapPVEShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("DamageLvGapPVE");
    if (!t) { SHINELOG_WARN("DamageLvGapPVE.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        DamageLvGapPVERow rec;
        rec.uiLvGap = (uint16)ShnGetU32(*t, _r, "LvGap");
        rec.uiDamageRate = (uint16)ShnGetU32(*t, _r, "DamageRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("DamageLvGapPVE.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
