// Server/Zone/GambleHouse/GBTaxRateTable.cpp
// FEATURE: casino-tax
#include "GBTaxRateTable.h"
#include "../../DataReader/ShnRegistry.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBTaxRateTable& GBTaxRateTable::Get() { static GBTaxRateTable s; return s; }

bool GBTaxRateTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-tax -- column read: GameType, GB_TaxRate,
    // GB_JPSave, GB_JPSaveRate
    const ShnFile* t = ShnRegistry::Get().GetTable("GBTaxRate");
    if (!t) { SHINELOG_WARN("GBTaxRate.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint32 gt = ShnGetU32(*t, i, "GameType");
        if (gt == 0) continue;
        GBTaxRow r;
        r.uiTaxRate    = (uint16)ShnGetU32(*t, i, "GB_TaxRate");
        r.bJPSave      = (uint8) ShnGetU32(*t, i, "GB_JPSave");
        r.uiJPSaveRate = (uint16)ShnGetU32(*t, i, "GB_JPSaveRate");
        m_kRows[gt] = r;
    }
    SHINELOG_INFO("GBTaxRate: %u game types", (uint32)m_kRows.size());
    return !m_kRows.empty();
}

bool GBTaxRateTable::FindByGameType(uint32 uiGameType, GBTaxRow& rOut) const {
    std::map<uint32, GBTaxRow>::const_iterator it = m_kRows.find(uiGameType);
    if (it == m_kRows.end()) return false;
    rOut = it->second;
    return true;
}

} // namespace fiesta
