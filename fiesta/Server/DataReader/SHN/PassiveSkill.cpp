// Server/DataReader/SHN/PassiveSkill.cpp
// Auto-generated: one-file-per-SHN split for PassiveSkill.shn
#include "PassiveSkill.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

PassiveSkillShn& PassiveSkillShn::Get() { static PassiveSkillShn s; return s; }

void PassiveSkillShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PassiveSkill");
    if (!t) { SHINELOG_WARN("PassiveSkill.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PassiveSkillRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.kName = ShnGetStr(*t, _r, "Name");
        rec.uiWeaponMastery = ShnGetU32(*t, _r, "WeaponMastery");
        rec.kDemandSk = ShnGetStr(*t, _r, "DemandSk");
        rec.uiMstRtTmp = ShnGetU32(*t, _r, "MstRtTmp");
        rec.uiMstRtSword1 = ShnGetU32(*t, _r, "MstRtSword1");
        rec.uiMstRtHammer1 = ShnGetU32(*t, _r, "MstRtHammer1");
        rec.uiMstRtSword2 = ShnGetU32(*t, _r, "MstRtSword2");
        rec.uiMstRtAxe2 = ShnGetU32(*t, _r, "MstRtAxe2");
        rec.uiMstRtMace1 = ShnGetU32(*t, _r, "MstRtMace1");
        rec.uiMstRtBow2 = ShnGetU32(*t, _r, "MstRtBow2");
        rec.uiMstRtCrossBow2 = ShnGetU32(*t, _r, "MstRtCrossBow2");
        rec.uiMstRtWand2 = ShnGetU32(*t, _r, "MstRtWand2");
        rec.uiMstRtStaff2 = ShnGetU32(*t, _r, "MstRtStaff2");
        rec.uiMstRtClaw = ShnGetU32(*t, _r, "MstRtClaw");
        rec.uiMstRtDSword = ShnGetU32(*t, _r, "MstRtDSword");
        rec.uiMstPlTmp = ShnGetU32(*t, _r, "MstPlTmp");
        rec.uiMstPlSword1 = ShnGetU32(*t, _r, "MstPlSword1");
        rec.uiMstPlHammer1 = ShnGetU32(*t, _r, "MstPlHammer1");
        rec.uiMstPlSword2 = ShnGetU32(*t, _r, "MstPlSword2");
        rec.uiMstPlAxe2 = ShnGetU32(*t, _r, "MstPlAxe2");
        rec.uiMstPlMace1 = ShnGetU32(*t, _r, "MstPlMace1");
        rec.uiMstPlBow2 = ShnGetU32(*t, _r, "MstPlBow2");
        rec.uiMstPlCrossBow2 = ShnGetU32(*t, _r, "MstPlCrossBow2");
        rec.uiMstPlWand2 = ShnGetU32(*t, _r, "MstPlWand2");
        rec.uiMstPlStaff2 = ShnGetU32(*t, _r, "MstPlStaff2");
        rec.uiMstPlClaw = ShnGetU32(*t, _r, "MstPlClaw");
        rec.uiMstPlDSword = ShnGetU32(*t, _r, "MstPlDSword");
        rec.uiSPRecover = ShnGetU32(*t, _r, "SPRecover");
        rec.uiTB = ShnGetU32(*t, _r, "TB");
        rec.uiMaxSP = ShnGetU32(*t, _r, "MaxSP");
        rec.uiMaxLP = ShnGetU32(*t, _r, "MaxLP");
        rec.uiIntel = ShnGetU32(*t, _r, "Intel");
        rec.uiCastingTime = ShnGetU32(*t, _r, "CastingTime");
        rec.uiMACriRate = (uint16)ShnGetU32(*t, _r, "MACriRate");
        rec.uiWCRateUp = ShnGetU32(*t, _r, "WCRateUp");
        rec.uiMARateUp = ShnGetU32(*t, _r, "MARateUp");
        rec.uiHpDownDamegeUp = (uint16)ShnGetU32(*t, _r, "HpDownDamegeUp");
        rec.uiDownDamegeHp = (uint16)ShnGetU32(*t, _r, "DownDamegeHp");
        rec.uiHpDownAcUp = (uint16)ShnGetU32(*t, _r, "HpDownAcUp");
        rec.uiDownAcHp = (uint16)ShnGetU32(*t, _r, "DownAcHp");
        rec.uiMoveTBUpPlus = (uint16)ShnGetU32(*t, _r, "MoveTBUpPlus");
        rec.uiHealUPRate = (uint16)ShnGetU32(*t, _r, "HealUPRate");
        rec.uiKeepTimeBuffUPRate = (uint16)ShnGetU32(*t, _r, "KeepTimeBuffUPRate");
        rec.uiCriDmgUpRate = (uint16)ShnGetU32(*t, _r, "CriDmgUpRate");
        rec.uiActiveSkillGroupInx = ShnGetU32(*t, _r, "ActiveSkillGroupInx");
        rec.uiDMG_MinusRate = (uint16)ShnGetU32(*t, _r, "DMG_MinusRate");
        m_kById[rec.uiID] = m_kRows.size();
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PassiveSkill.shn: %u rows", (uint32)m_kRows.size());
}

const PassiveSkillRow* PassiveSkillShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const PassiveSkillRow* PassiveSkillShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
