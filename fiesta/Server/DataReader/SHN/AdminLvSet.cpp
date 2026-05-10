// Server/DataReader/SHN/AdminLvSet.cpp
// Auto-generated: one-file-per-SHN split for AdminLvSet.shn
#include "AdminLvSet.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

AdminLvSetShn& AdminLvSetShn::Get() { static AdminLvSetShn s; return s; }

void AdminLvSetShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("AdminLvSet");
    if (!t) { SHINELOG_WARN("AdminLvSet.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        AdminLvSetRow rec;
        rec.kALS_Cmd = ShnGetStr(*t, _r, "ALS_Cmd");
        rec.uiALS_Lv = (uint8)ShnGetU32(*t, _r, "ALS_Lv");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AdminLvSet.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
