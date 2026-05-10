// Server/DataReader/SHN/GuildGradeScoreData.cpp
// Auto-generated: one-file-per-SHN split for GuildGradeScoreData.shn
#include "GuildGradeScoreData.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GuildGradeScoreDataShn& GuildGradeScoreDataShn::Get() { static GuildGradeScoreDataShn s; return s; }

void GuildGradeScoreDataShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildGradeScoreData");
    if (!t) { SHINELOG_WARN("GuildGradeScoreData.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildGradeScoreDataRow rec;
        rec.iGradeScore = (int16)ShnGetI32(*t, _r, "GradeScore");
        rec.iUnkCol1 = (int16)ShnGetI32(*t, _r, "UnkCol1");
        rec.iUnkCol2 = (int16)ShnGetI32(*t, _r, "UnkCol2");
        rec.iUnkCol3 = (int16)ShnGetI32(*t, _r, "UnkCol3");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = (int16)ShnGetI32(*t, _r, "UnkCol6");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildGradeScoreData.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
