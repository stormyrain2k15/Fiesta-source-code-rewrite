// Server/DataReader/SHN/BRAccUpgradeInfo.cpp
// Auto-generated: one-file-per-SHN split for BRAccUpgradeInfo.shn
#include "BRAccUpgradeInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

BRAccUpgradeInfoShn& BRAccUpgradeInfoShn::Get() { static BRAccUpgradeInfoShn s; return s; }

void BRAccUpgradeInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("BRAccUpgradeInfo");
    if (!t) { SHINELOG_WARN("BRAccUpgradeInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        BRAccUpgradeInfoRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiUpFactor = ShnGetU32(*t, _r, "UpFactor");
        rec.iBRAccUpdata = (int16)ShnGetI32(*t, _r, "BRAccUpdata");
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
        rec.iUnkCol15 = (int16)ShnGetI32(*t, _r, "UnkCol15");
        rec.iUnkCol16 = (int16)ShnGetI32(*t, _r, "UnkCol16");
        rec.iUnkCol17 = (int16)ShnGetI32(*t, _r, "UnkCol17");
        rec.iUnkCol18 = (int16)ShnGetI32(*t, _r, "UnkCol18");
        rec.iUnkCol19 = (int16)ShnGetI32(*t, _r, "UnkCol19");
        rec.iUnkCol20 = (int16)ShnGetI32(*t, _r, "UnkCol20");
        rec.iUnkCol21 = (int16)ShnGetI32(*t, _r, "UnkCol21");
        rec.iUnkCol22 = (int16)ShnGetI32(*t, _r, "UnkCol22");
        rec.iUnkCol23 = (int16)ShnGetI32(*t, _r, "UnkCol23");
        m_kById[rec.uiID] = m_kRows.size();
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("BRAccUpgradeInfo.shn: %u rows", (uint32)m_kRows.size());
}

const BRAccUpgradeInfoRow* BRAccUpgradeInfoShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const BRAccUpgradeInfoRow* BRAccUpgradeInfoShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
