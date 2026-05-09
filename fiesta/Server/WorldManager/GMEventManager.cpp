// Server/WorldManager/GMEventManager.cpp
// GM event coordination. Walks the GMEvent table (loaded from
// GMEvent.shn at boot) once per tick, watches for [start, end] window
// transitions, and broadcasts NC_INTER_BROADCAST_CMD with a {kind=2,
// eventNo, action} body to every zone. Zone-side handlers translate the
// broadcast to client-visible chat / SFX / loot-rate boost.
#include "WMServices.h"
#include "WorldManagerServer.h"
#include "../DataReader/ShnRegistry.h"
#include "../Common/NETCOMMAND.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

GMEventManager& GMEventManager::Get() { static GMEventManager s; return s; }

static std::map<uint32, bool> g_kRunning;

static uint16 NowHHMM() {
    SYSTEMTIME st; GetLocalTime(&st);
    return (uint16)(st.wHour * 100 + st.wMinute);
}

static bool InWindow(uint16 uiNow, int32 iStart, int32 iEnd) {
    if (iStart == iEnd) return false;
    if (iStart < iEnd) return uiNow >= iStart && uiNow < iEnd;
    return uiNow >= iStart || uiNow < iEnd;
}

void GMEventManager::Tick() {
    const ShnFile* pkT = ShnRegistry::Get().GetTable("GMEvent");
    if (!pkT) return;
    uint16 uiHHMM = NowHHMM();
    for (uint32 i = 0; i < pkT->RecordCount(); ++i) {
        int32 iEventNo = ShnGetI32(*pkT, i, "EventNo");
        int32 iStart   = ShnGetI32(*pkT, i, "StartTime");
        int32 iEnd     = ShnGetI32(*pkT, i, "EndTime");
        bool  bActive  = InWindow(uiHHMM, iStart, iEnd);
        std::map<uint32, bool>::iterator it = g_kRunning.find((uint32)iEventNo);
        bool bWas = (it != g_kRunning.end()) ? it->second : false;
        if (bActive == bWas) continue;
        g_kRunning[(uint32)iEventNo] = bActive;

        PacketBuffer body;
        body.WriteU8(2);                       // kind = GM event
        body.WriteU32((uint32)iEventNo);
        body.WriteU8(bActive ? 1 : 0);         // 1 = start, 0 = end
        WorldManagerServer::Get().BroadcastToZones(NC_INTER_BROADCAST_CMD,
                                                   body.Data(), body.Size());
        SHINELOG_INFO("GMEvent #%d %s", iEventNo, bActive ? "START" : "END");
    }
}

} // namespace fiesta
