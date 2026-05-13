// Server/DataReader/SHN/FieldLvCondition.cpp
// Auto-generated: one-file-per-SHN split for FieldLvCondition.shn
#include "FieldLvCondition.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

FieldLvConditionShn& FieldLvConditionShn::Get() { static FieldLvConditionShn s; return s; }

void FieldLvConditionShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("FieldLvCondition");
    if (!t) { SHINELOG_WARN("FieldLvCondition.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        FieldLvConditionRow rec;
        rec.kMapIndex = ShnGetStr(*t, _r, "MapIndex");
        rec.uiModeIDLv = (uint8)ShnGetU32(*t, _r, "ModeIDLv");
        rec.uiLvFrom = (uint16)ShnGetU32(*t, _r, "LvFrom");
        rec.uiLvTo = (uint16)ShnGetU32(*t, _r, "LvTo");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("FieldLvCondition.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
