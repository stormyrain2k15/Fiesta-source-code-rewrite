// Server/Zone/GambleHouse/GBBanTime.cpp
// FEATURE: casino-bantime
#include "GBAuxTables.h"
#include "../../DataReader/ShnRegistry.h"
#include "../../Shared/ShineLogSystem.h"
namespace fiesta {
GBBanTimeTable& GBBanTimeTable::Get() { static GBBanTimeTable s; return s; }
bool GBBanTimeTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-bantime -- column read: GameType, GB_Ban, GB_BanTime
    const ShnFile* t = ShnRegistry::Get().GetTable("GBBanTime");
    if (!t) { SHINELOG_WARN("GBBanTime.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint32 gt    = ShnGetU32(*t, i, "GameType");
        uint32 banFl = ShnGetU32(*t, i, "GB_Ban");
        uint32 ms    = ShnGetU32(*t, i, "GB_BanTime");
        if (banFl == 0) continue;
        m_kRows[gt] = ms;
    }
    SHINELOG_INFO("GBBanTime: %u game types", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
uint32 GBBanTimeTable::BanMs(uint32 uiGT) const {
    std::map<uint32, uint32>::const_iterator it = m_kRows.find(uiGT);
    return (it == m_kRows.end()) ? 0 : it->second;
}
} // namespace fiesta
