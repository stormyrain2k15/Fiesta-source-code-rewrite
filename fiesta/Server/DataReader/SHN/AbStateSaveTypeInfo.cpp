// Server/DataReader/SHN/AbStateSaveTypeInfo.cpp
// Auto-generated: one-file-per-SHN split for AbStateSaveTypeInfo.shn
#include "AbStateSaveTypeInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

AbStateSaveTypeInfoShn& AbStateSaveTypeInfoShn::Get() { static AbStateSaveTypeInfoShn s; return s; }

void AbStateSaveTypeInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("AbStateSaveTypeInfo");
    if (!t) { SHINELOG_WARN("AbStateSaveTypeInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        AbStateSaveTypeInfoRow rec;
        rec.uiAbStateSaveType = ShnGetU32(*t, _r, "AbStateSaveType");
        rec.uiIsSaveLink = (uint8)ShnGetU32(*t, _r, "IsSaveLink");
        rec.uiIsSaveDie = (uint8)ShnGetU32(*t, _r, "IsSaveDie");
        rec.uiIsSaveLogoff = (uint8)ShnGetU32(*t, _r, "IsSaveLogoff");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AbStateSaveTypeInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
