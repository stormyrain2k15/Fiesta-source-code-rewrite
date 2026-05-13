// Server/Zone/GambleHouse/GBHouseTable.cpp
// FEATURE: casino-house
#include "GBHouseTable.h"
#include "../../DataReader/ShnRegistry.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBHouseTable& GBHouseTable::Get() { static GBHouseTable s; return s; }

GBHouseTable::GBHouseTable() : m_bLoaded(false) {
    // Sane fallbacks until Bind() runs. 10 gold per chip, 20% tax,
    // daily reset at 06:00 server-local (canon NA2016 reset hour).
    m_kRow.uiGameMoney   = 10;
    m_kRow.uiExchangeTax = 20;
    m_kRow.uiResetH = 6;
    m_kRow.uiResetM = 0;
    m_kRow.uiResetS = 0;
}

bool GBHouseTable::Bind() {
    // FEATURE: casino-house -- column read: GB_GameMoney,
    // GB_ExchangeTax, GB_ResetTimeHour/Min/Sec
    const ShnFile* t = ShnRegistry::Get().GetTable("GBHouse");
    if (!t || t->Rows().empty()) {
        SHINELOG_WARN("GBHouseTable: GBHouse.shn missing -- using defaults "
                      "(rate=%u tax=%u%% reset=%02u:%02u:%02u)",
                      m_kRow.uiGameMoney, m_kRow.uiExchangeTax,
                      m_kRow.uiResetH, m_kRow.uiResetM, m_kRow.uiResetS);
        return false;
    }
    const size_t r = 0;     // single-row global config
    m_kRow.uiGameMoney   = ShnGetU32(*t, r, "GB_GameMoney");
    m_kRow.uiExchangeTax = ShnGetU32(*t, r, "GB_ExchangeTax");
    m_kRow.uiResetH = (uint8)ShnGetU32(*t, r, "GB_ResetTimeHour");
    m_kRow.uiResetM = (uint8)ShnGetU32(*t, r, "GB_ResetTimeMin");
    m_kRow.uiResetS = (uint8)ShnGetU32(*t, r, "GB_ResetTimeSec");
    if (m_kRow.uiGameMoney == 0) m_kRow.uiGameMoney = 10;
    m_bLoaded = true;
    SHINELOG_INFO("GBHouseTable: rate=%u gold/chip tax=%u%% reset=%02u:%02u:%02u",
                  m_kRow.uiGameMoney, m_kRow.uiExchangeTax,
                  m_kRow.uiResetH, m_kRow.uiResetM, m_kRow.uiResetS);
    return true;
}

} // namespace shine
