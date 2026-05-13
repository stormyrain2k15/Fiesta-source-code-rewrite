// Server/DataReader/SHN/WeaponAttrib.cpp
// Auto-generated: one-file-per-SHN split for WeaponAttrib.shn
#include "WeaponAttrib.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

WeaponAttribShn& WeaponAttribShn::Get() { static WeaponAttribShn s; return s; }

void WeaponAttribShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("WeaponAttrib");
    if (!t) { SHINELOG_WARN("WeaponAttrib.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        WeaponAttribRow rec;
        rec.uiWeaponType = ShnGetU32(*t, _r, "WeaponType");
        rec.uiUsableDegree = (uint16)ShnGetU32(*t, _r, "UsableDegree");
        rec.uiIsUsableInMoving = (uint8)ShnGetU32(*t, _r, "IsUsableInMoving");
        rec.iHitRate = (int16)ShnGetI32(*t, _r, "HitRate");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("WeaponAttrib.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
