// Server/DataReader/SHN/FaceInfo.cpp
// Auto-generated: one-file-per-SHN split for FaceInfo.shn
#include "FaceInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

FaceInfoShn& FaceInfoShn::Get() { static FaceInfoShn s; return s; }

void FaceInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("FaceInfo");
    if (!t) { SHINELOG_WARN("FaceInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        FaceInfoRow rec;
        rec.uiID = (uint8)ShnGetU32(*t, _r, "ID");
        rec.kFaceName = ShnGetStr(*t, _r, "FaceName");
        rec.uiGrade = (uint8)ShnGetU32(*t, _r, "Grade");
        rec.uiFM_F_Male = (uint8)ShnGetU32(*t, _r, "FM_F_Male");
        rec.uiFT_F_Male = (uint8)ShnGetU32(*t, _r, "FT_F_Male");
        rec.uiFM_F_Female = (uint8)ShnGetU32(*t, _r, "FM_F_Female");
        rec.uiFT_F_Female = (uint8)ShnGetU32(*t, _r, "FT_F_Female");
        rec.uiFM_C_Male = (uint8)ShnGetU32(*t, _r, "FM_C_Male");
        rec.uiFT_C_Male = (uint8)ShnGetU32(*t, _r, "FT_C_Male");
        rec.uiFM_C_Female = (uint8)ShnGetU32(*t, _r, "FM_C_Female");
        rec.uiFT_C_Female = (uint8)ShnGetU32(*t, _r, "FT_C_Female");
        rec.uiFM_A_Male = (uint8)ShnGetU32(*t, _r, "FM_A_Male");
        rec.uiFT_A_Male = (uint8)ShnGetU32(*t, _r, "FT_A_Male");
        rec.uiFM_A_Female = (uint8)ShnGetU32(*t, _r, "FM_A_Female");
        rec.uiFT_A_Female = (uint8)ShnGetU32(*t, _r, "FT_A_Female");
        rec.uiFM_M_Male = (uint8)ShnGetU32(*t, _r, "FM_M_Male");
        rec.uiFT_M_Male = (uint8)ShnGetU32(*t, _r, "FT_M_Male");
        rec.uiFM_M_Female = (uint8)ShnGetU32(*t, _r, "FM_M_Female");
        rec.uiFT_M_Female = (uint8)ShnGetU32(*t, _r, "FT_M_Female");
        rec.uiFM_J_Male = (uint8)ShnGetU32(*t, _r, "FM_J_Male");
        rec.uiFT_J_Male = (uint8)ShnGetU32(*t, _r, "FT_J_Male");
        rec.uiFM_J_Female = (uint8)ShnGetU32(*t, _r, "FM_J_Female");
        rec.uiFT_J_Female = (uint8)ShnGetU32(*t, _r, "FT_J_Female");
        rec.uiFM_S_Male = (uint8)ShnGetU32(*t, _r, "FM_S_Male");
        rec.uiFT_S_Male = (uint8)ShnGetU32(*t, _r, "FT_S_Male");
        rec.uiFM_S_Female = (uint8)ShnGetU32(*t, _r, "FM_S_Female");
        rec.uiFT_S_Female = (uint8)ShnGetU32(*t, _r, "FT_S_Female");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("FaceInfo.shn: %u rows", (uint32)m_kRows.size());
}

const FaceInfoRow* FaceInfoShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
