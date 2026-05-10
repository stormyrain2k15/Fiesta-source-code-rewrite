// Server/DataReader/SHN/UpgradeInfo.cpp
// Auto-generated: one-file-per-SHN split for UpgradeInfo.shn
#include "UpgradeInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

UpgradeInfoShn& UpgradeInfoShn::Get() { static UpgradeInfoShn s; return s; }

void UpgradeInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("UpgradeInfo");
    if (!t) { SHINELOG_WARN("UpgradeInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        UpgradeInfoRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiUpFactor = ShnGetU32(*t, _r, "UpFactor");
        rec.iUpdata = (int16)ShnGetI32(*t, _r, "Updata");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = (int16)ShnGetI32(*t, _r, "UnkCol6");
        rec.iUnkCol7 = (int16)ShnGetI32(*t, _r, "UnkCol7");
        rec.iUnkCol8 = (int16)ShnGetI32(*t, _r, "UnkCol8");
        rec.iUnkCol9 = (int16)ShnGetI32(*t, _r, "UnkCol9");
        rec.iUnkCol10 = (int16)ShnGetI32(*t, _r, "UnkCol10");
        rec.iUnkCol11 = (int16)ShnGetI32(*t, _r, "UnkCol11");
        rec.iUnkCol12 = (int16)ShnGetI32(*t, _r, "UnkCol12");
        rec.iUnkCol13 = (int16)ShnGetI32(*t, _r, "UnkCol13");
        rec.iUnkCol14 = (int16)ShnGetI32(*t, _r, "UnkCol14");
        m_kById[rec.uiID] = m_kRows.size();
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("UpgradeInfo.shn: %u rows", (uint32)m_kRows.size());
}

const UpgradeInfoRow* UpgradeInfoShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const UpgradeInfoRow* UpgradeInfoShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
