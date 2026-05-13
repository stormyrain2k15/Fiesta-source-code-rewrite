// Server/WorldManager/DailyQuestTimer.cpp
// Daily/weekly/monthly quest reset scheduler. Fires at the configured
// wall-clock boundary (local midnight by default) and broadcasts the new
// day key to every connected zone, plus enqueues a CharDB-side reset via
// WMCharDBClient::DailyResetTick (-> p_Daily_Reset). Per-quest reset cadence
// (daily / weekly / monthly) is decided by the table on the CharDB side.
#include "WMServices.h"
#include "WorldManagerServer.h"
#include "WMCharDBClient.h"
#include "../Common/NETCOMMAND.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

DailyQuestTimer& DailyQuestTimer::Get() { static DailyQuestTimer s; return s; }

static uint32 LocalDayKey() {
    SYSTEMTIME st; GetLocalTime(&st);
    return (uint32)st.wYear * 10000u + (uint32)st.wMonth * 100u + (uint32)st.wDay;
}

void DailyQuestTimer::Tick() {
    uint32 uiToday = LocalDayKey();
    if (m_uiLastFiredDay == 0) { m_uiLastFiredDay = uiToday; return; }
    if (uiToday == m_uiLastFiredDay) return;
    m_uiLastFiredDay = uiToday;

    SHINELOG_INFO("DailyReset firing for day=%u", uiToday);
    WMCharDBClient::Get().DailyResetTick(uiToday);

    // Inter-broadcast envelope: kind=3 = daily reset.
    PacketBuffer body;
    body.WriteU8(3);                    // kind = daily reset
    body.WriteU32(uiToday);             // YYYYMMDD day key
    WorldManagerServer::Get().BroadcastToZones(NC_INTER_BROADCAST_CMD,
                                               body.Data(), body.Size());
}

} // namespace shine
