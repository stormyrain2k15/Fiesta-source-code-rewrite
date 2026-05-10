// Server/DataReader/SHN/AccUpGradeInfo.cpp
// Auto-generated: one-file-per-SHN split for AccUpGradeInfo.shn
#include "AccUpGradeInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

AccUpGradeInfoShn& AccUpGradeInfoShn::Get() { static AccUpGradeInfoShn s; return s; }

void AccUpGradeInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("AccUpGradeInfo");
    if (!t) { SHINELOG_WARN("AccUpGradeInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        AccUpGradeInfoRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiUpFactor = ShnGetU32(*t, _r, "UpFactor");
        rec.uiSuccess = (uint16)ShnGetU32(*t, _r, "Success");
        rec.uiMin1 = (uint16)ShnGetU32(*t, _r, "Min1");
        rec.uiMax1 = (uint16)ShnGetU32(*t, _r, "Max1");
        rec.uiLimit1 = (uint16)ShnGetU32(*t, _r, "Limit1");
        rec.uiMin2 = (uint16)ShnGetU32(*t, _r, "Min2");
        rec.uiMax2 = (uint16)ShnGetU32(*t, _r, "Max2");
        rec.uiLimit2 = (uint16)ShnGetU32(*t, _r, "Limit2");
        rec.uiMin3 = (uint16)ShnGetU32(*t, _r, "Min3");
        rec.uiMax3 = (uint16)ShnGetU32(*t, _r, "Max3");
        rec.uiLimit3 = (uint16)ShnGetU32(*t, _r, "Limit3");
        rec.uiMin4 = (uint16)ShnGetU32(*t, _r, "Min4");
        rec.uiMax4 = (uint16)ShnGetU32(*t, _r, "Max4");
        rec.uiLimit4 = (uint16)ShnGetU32(*t, _r, "Limit4");
        rec.uiMin5 = (uint16)ShnGetU32(*t, _r, "Min5");
        rec.uiMax5 = (uint16)ShnGetU32(*t, _r, "Max5");
        rec.uiLimit5 = (uint16)ShnGetU32(*t, _r, "Limit5");
        rec.uiMin6 = (uint16)ShnGetU32(*t, _r, "Min6");
        rec.uiMax6 = (uint16)ShnGetU32(*t, _r, "Max6");
        rec.uiLimit6 = (uint16)ShnGetU32(*t, _r, "Limit6");
        rec.uiMin7 = (uint16)ShnGetU32(*t, _r, "Min7");
        rec.uiMax7 = (uint16)ShnGetU32(*t, _r, "Max7");
        rec.uiLimit7 = (uint16)ShnGetU32(*t, _r, "Limit7");
        rec.uiMin8 = (uint16)ShnGetU32(*t, _r, "Min8");
        rec.uiMax8 = (uint16)ShnGetU32(*t, _r, "Max8");
        rec.uiLimit8 = (uint16)ShnGetU32(*t, _r, "Limit8");
        rec.uiMin9 = (uint16)ShnGetU32(*t, _r, "Min9");
        rec.uiMax9 = (uint16)ShnGetU32(*t, _r, "Max9");
        rec.uiLimit9 = (uint16)ShnGetU32(*t, _r, "Limit9");
        rec.uiMin10 = (uint16)ShnGetU32(*t, _r, "Min10");
        rec.uiMax10 = (uint16)ShnGetU32(*t, _r, "Max10");
        rec.uiLimit10 = (uint16)ShnGetU32(*t, _r, "Limit10");
        rec.uiMin11 = (uint16)ShnGetU32(*t, _r, "Min11");
        rec.uiMax11 = (uint16)ShnGetU32(*t, _r, "Max11");
        rec.uiLimit11 = (uint16)ShnGetU32(*t, _r, "Limit11");
        rec.uiMin12 = (uint16)ShnGetU32(*t, _r, "Min12");
        rec.uiMax12 = (uint16)ShnGetU32(*t, _r, "Max12");
        rec.uiLimit12 = (uint16)ShnGetU32(*t, _r, "Limit12");
        m_kById[rec.uiID] = m_kRows.size();
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AccUpGradeInfo.shn: %u rows", (uint32)m_kRows.size());
}

const AccUpGradeInfoRow* AccUpGradeInfoShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const AccUpGradeInfoRow* AccUpGradeInfoShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
