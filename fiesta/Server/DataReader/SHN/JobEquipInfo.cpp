// Server/DataReader/SHN/JobEquipInfo.cpp
// Auto-generated: one-file-per-SHN split for JobEquipInfo.shn
#include "JobEquipInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

JobEquipInfoShn& JobEquipInfoShn::Get() { static JobEquipInfoShn s; return s; }

void JobEquipInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("JobEquipInfo");
    if (!t) { SHINELOG_WARN("JobEquipInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        JobEquipInfoRow rec;
        rec.uiChrClass = ShnGetU32(*t, _r, "ChrClass");
        rec.kEqu_RightHand = ShnGetStr(*t, _r, "Equ_RightHand");
        rec.kEqu_LeftHand = ShnGetStr(*t, _r, "Equ_LeftHand");
        rec.kEqu_Shoes = ShnGetStr(*t, _r, "Equ_Shoes");
        rec.kEqu_Head = ShnGetStr(*t, _r, "Equ_Head");
        rec.kEqu_Leg = ShnGetStr(*t, _r, "Equ_Leg");
        rec.kEqu_Body = ShnGetStr(*t, _r, "Equ_Body");
        rec.kEqu_ETC = ShnGetStr(*t, _r, "Equ_ETC");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("JobEquipInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
