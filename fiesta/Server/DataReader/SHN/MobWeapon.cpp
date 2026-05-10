// Server/DataReader/SHN/MobWeapon.cpp
// Auto-generated: one-file-per-SHN split for MobWeapon.shn
#include "MobWeapon.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MobWeaponShn& MobWeaponShn::Get() { static MobWeaponShn s; return s; }

void MobWeaponShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobWeapon");
    if (!t) { SHINELOG_WARN("MobWeapon.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobWeaponRow rec;
        rec.uiID = ShnGetU32(*t, _r, "ID");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.kSkill = ShnGetStr(*t, _r, "Skill");
        rec.uiAtkSpd = (uint16)ShnGetU32(*t, _r, "AtkSpd");
        rec.uiBlastRate = (uint16)ShnGetU32(*t, _r, "BlastRate");
        rec.uiAtkDly = (uint16)ShnGetU32(*t, _r, "AtkDly");
        rec.uiSwingTime = (uint16)ShnGetU32(*t, _r, "SwingTime");
        rec.uiHitTime = (uint16)ShnGetU32(*t, _r, "HitTime");
        rec.uiAtkType = ShnGetU32(*t, _r, "AtkType");
        rec.uiMinWC = ShnGetU32(*t, _r, "MinWC");
        rec.uiMaxWC = ShnGetU32(*t, _r, "MaxWC");
        rec.uiTH = (uint16)ShnGetU32(*t, _r, "TH");
        rec.uiMinMA = ShnGetU32(*t, _r, "MinMA");
        rec.uiMaxMA = ShnGetU32(*t, _r, "MaxMA");
        rec.uiMH = (uint16)ShnGetU32(*t, _r, "MH");
        rec.uiRange = (uint16)ShnGetU32(*t, _r, "Range");
        rec.uiMopAttackTarget = ShnGetU32(*t, _r, "MopAttackTarget");
        rec.uiHitType = ShnGetU32(*t, _r, "HitType");
        rec.kStaName = ShnGetStr(*t, _r, "StaName");
        rec.uiStaStrength = (uint16)ShnGetU32(*t, _r, "StaStrength");
        rec.uiStaRate = (uint16)ShnGetU32(*t, _r, "StaRate");
        rec.uiAggroInitialize = (uint16)ShnGetU32(*t, _r, "AggroInitialize");
        m_kById[rec.uiID] = m_kRows.size();
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobWeapon.shn: %u rows", (uint32)m_kRows.size());
}

const MobWeaponRow* MobWeaponShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const MobWeaponRow* MobWeaponShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
