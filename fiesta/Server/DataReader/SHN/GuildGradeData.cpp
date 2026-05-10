// Server/DataReader/SHN/GuildGradeData.cpp
// Auto-generated: one-file-per-SHN split for GuildGradeData.shn
#include "GuildGradeData.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GuildGradeDataShn& GuildGradeDataShn::Get() { static GuildGradeDataShn s; return s; }

void GuildGradeDataShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildGradeData");
    if (!t) { SHINELOG_WARN("GuildGradeData.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildGradeDataRow rec;
        rec.uiType = (uint8)ShnGetU32(*t, _r, "Type");
        rec.uiNeedFame = ShnGetU32(*t, _r, "NeedFame");
        rec.uiMaxOfMember = (uint16)ShnGetU32(*t, _r, "MaxOfMember");
        rec.iMaxOfGradeMember = (int16)ShnGetI32(*t, _r, "MaxOfGradeMember");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = (int16)ShnGetI32(*t, _r, "UnkCol6");
        rec.iUnkCol7 = (int16)ShnGetI32(*t, _r, "UnkCol7");
        rec.iUnkCol8 = (int16)ShnGetI32(*t, _r, "UnkCol8");
        rec.iUnkCol9 = (int16)ShnGetI32(*t, _r, "UnkCol9");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildGradeData.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
