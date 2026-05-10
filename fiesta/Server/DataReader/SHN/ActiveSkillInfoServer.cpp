// Server/DataReader/SHN/ActiveSkillInfoServer.cpp
// Auto-generated: one-file-per-SHN split for ActiveSkillInfoServer.shn
#include "ActiveSkillInfoServer.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ActiveSkillInfoServerShn& ActiveSkillInfoServerShn::Get() { static ActiveSkillInfoServerShn s; return s; }

void ActiveSkillInfoServerShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ActiveSkillInfoServer");
    if (!t) { SHINELOG_WARN("ActiveSkillInfoServer.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ActiveSkillInfoServerRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiUsualAttack = (uint8)ShnGetU32(*t, _r, "UsualAttack");
        rec.uiSkilPyHitRate = ShnGetU32(*t, _r, "SkilPyHitRate");
        rec.uiSkilMaHitRate = ShnGetU32(*t, _r, "SkilMaHitRate");
        rec.uiPsySucRate = ShnGetU32(*t, _r, "PsySucRate");
        rec.uiMagSucRate = ShnGetU32(*t, _r, "MagSucRate");
        rec.uiStaLevel = (uint8)ShnGetU32(*t, _r, "StaLevel");
        rec.uiDmgIncRate = ShnGetU32(*t, _r, "DmgIncRate");
        rec.uiDmgIncValue = (uint16)ShnGetU32(*t, _r, "DmgIncValue");
        rec.uiSkillHitType = ShnGetU32(*t, _r, "SkillHitType");
        rec.uiItremUseSkill = (uint8)ShnGetU32(*t, _r, "ItremUseSkill");
        rec.uiAggroPerDamage = ShnGetU32(*t, _r, "AggroPerDamage");
        rec.uiAbsoluteAggro = ShnGetU32(*t, _r, "AbsoluteAggro");
        rec.uiAttackStart = (uint8)ShnGetU32(*t, _r, "AttackStart");
        rec.uiAttackEnd = (uint8)ShnGetU32(*t, _r, "AttackEnd");
        rec.uiSwingTime = (uint16)ShnGetU32(*t, _r, "SwingTime");
        rec.uiHitTime = (uint16)ShnGetU32(*t, _r, "HitTime");
        rec.uiAddSoul = (uint8)ShnGetU32(*t, _r, "AddSoul");
        m_kById[rec.uiID] = m_kRows.size();
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ActiveSkillInfoServer.shn: %u rows", (uint32)m_kRows.size());
}

const ActiveSkillInfoServerRow* ActiveSkillInfoServerShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const ActiveSkillInfoServerRow* ActiveSkillInfoServerShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
