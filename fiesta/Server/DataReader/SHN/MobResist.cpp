// Server/DataReader/SHN/MobResist.cpp
// Auto-generated: one-file-per-SHN split for MobResist.shn
#include "MobResist.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MobResistShn& MobResistShn::Get() { static MobResistShn s; return s; }

void MobResistShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobResist");
    if (!t) { SHINELOG_WARN("MobResist.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobResistRow rec;
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiResDot = (uint16)ShnGetU32(*t, _r, "ResDot");
        rec.uiResStun = (uint16)ShnGetU32(*t, _r, "ResStun");
        rec.uiResMoveSpeed = (uint16)ShnGetU32(*t, _r, "ResMoveSpeed");
        rec.uiResFear = (uint16)ShnGetU32(*t, _r, "ResFear");
        rec.uiResBinding = (uint16)ShnGetU32(*t, _r, "ResBinding");
        rec.uiResReverse = (uint16)ShnGetU32(*t, _r, "ResReverse");
        rec.uiResMesmerize = (uint16)ShnGetU32(*t, _r, "ResMesmerize");
        rec.uiResSeverBone = (uint16)ShnGetU32(*t, _r, "ResSeverBone");
        rec.uiResKnockBack = (uint16)ShnGetU32(*t, _r, "ResKnockBack");
        rec.uiResTBMinus = (uint16)ShnGetU32(*t, _r, "ResTBMinus");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobResist.shn: %u rows", (uint32)m_kRows.size());
}

const MobResistRow* MobResistShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
