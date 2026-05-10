// Server/DataReader/SHN/BadNameFilter.cpp
// Auto-generated: one-file-per-SHN split for BadNameFilter.shn
#include "BadNameFilter.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

BadNameFilterShn& BadNameFilterShn::Get() { static BadNameFilterShn s; return s; }

void BadNameFilterShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("BadNameFilter");
    if (!t) { SHINELOG_WARN("BadNameFilter.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        BadNameFilterRow rec;
        rec.kBadName = ShnGetStr(*t, _r, "BadName");
        rec.uiType = ShnGetU32(*t, _r, "Type");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("BadNameFilter.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
