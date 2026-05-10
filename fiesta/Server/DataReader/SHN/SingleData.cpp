// Server/DataReader/SHN/SingleData.cpp
// Auto-generated: one-file-per-SHN split for SingleData.shn
#include "SingleData.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

SingleDataShn& SingleDataShn::Get() { static SingleDataShn s; return s; }

void SingleDataShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("SingleData");
    if (!t) { SHINELOG_WARN("SingleData.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        SingleDataRow rec;
        rec.kSingleDataIDX = ShnGetStr(*t, _r, "SingleDataIDX");
        rec.uiSingleDataValue = (uint16)ShnGetU32(*t, _r, "SingleDataValue");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("SingleData.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
