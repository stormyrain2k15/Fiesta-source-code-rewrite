// Server/Zone/Tables/WeaponAttribTable.cpp
// FEATURE: world-creation -- WeaponAttrib.shn binder.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace shine {

WeaponAttribTable& WeaponAttribTable::Get() { static WeaponAttribTable s; return s; }

void WeaponAttribTable::Bind() {
    // FEATURE: world-creation -- column read: WeaponType, UsableDegree,
    // IsUsableInMoving, HitRate
    BIND_BEGIN(t, "WeaponAttrib")
    ITER_ROWS(t) {
        Row r;
        r.uiWeaponType       = ShnGetU32(*t, _r, "WeaponType");
        r.uiUsableDegree     = ShnGetU32(*t, _r, "UsableDegree");
        r.uiIsUsableInMoving = ShnGetU32(*t, _r, "IsUsableInMoving");
        r.uiHitRate          = ShnGetU32(*t, _r, "HitRate");
        m_kByType[r.uiWeaponType] = m_kRows.size();
        m_kRows.push_back(r);
    }
}

const WeaponAttribTable::Row* WeaponAttribTable::Find(uint32 t) const {
    std::map<uint32, size_t>::const_iterator it = m_kByType.find(t);
    return (it == m_kByType.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
