// Server/DataReader/SHN/EnchantSocketRate.cpp
// Auto-generated: one-file-per-SHN split for EnchantSocketRate.shn
#include "EnchantSocketRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

EnchantSocketRateShn& EnchantSocketRateShn::Get() { static EnchantSocketRateShn s; return s; }

void EnchantSocketRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("EnchantSocketRate");
    if (!t) { SHINELOG_WARN("EnchantSocketRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        EnchantSocketRateRow rec;
        rec.uiItemGradeType = ShnGetU32(*t, _r, "ItemGradeType");
        rec.uiSocket0 = (uint16)ShnGetU32(*t, _r, "Socket0");
        rec.uiSocket1 = (uint16)ShnGetU32(*t, _r, "Socket1");
        rec.uiSocket2 = (uint16)ShnGetU32(*t, _r, "Socket2");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("EnchantSocketRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
