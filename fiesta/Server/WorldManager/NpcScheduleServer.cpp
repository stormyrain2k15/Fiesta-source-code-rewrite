// Server/WorldManager/NpcScheduleServer.cpp
// Time-based NPC spawn/despawn. Walks NpcSchedule.shn rows on every Tick
// (each row: NpcID + start-HHMM + end-HHMM + map) and pushes a Spawn or
// Despawn intent to whichever zone hosts the target map. The schedule
// itself is global and authoritative on the WM; per-zone NPCSystem hooks
// pick up the broadcast and add/remove the spawn from its local registry.
#include "WMServices.h"
#include "WorldManagerServer.h"
#include "../DataReader/ShnRegistry.h"
#include "../Common/NETCOMMAND.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

NpcScheduleServer& NpcScheduleServer::Get() { static NpcScheduleServer s; return s; }

// rowKey -> active flag, so we only emit on transitions.
static std::map<uint32, bool> g_kRowActive;

static uint16 NowHHMM() {
    SYSTEMTIME st; GetLocalTime(&st);
    return (uint16)(st.wHour * 100 + st.wMinute);
}

static bool InWindow(uint16 uiNow, int32 iStart, int32 iEnd) {
    if (iStart == iEnd) return false;
    if (iStart < iEnd) return uiNow >= iStart && uiNow < iEnd;
    // Wrap (e.g. 22:00 -> 06:00)
    return uiNow >= iStart || uiNow < iEnd;
}

void NpcScheduleServer::Tick() {
    const ShnFile* pkT = ShnRegistry::Get().GetTable("NpcSchedule");
    if (!pkT) return;
    uint16 uiHHMM = NowHHMM();
    for (uint32 i = 0; i < pkT->RecordCount(); ++i) {
        int32 iNpcID  = ShnGetI32(*pkT, i, "NpcID");
        int32 iStart  = ShnGetI32(*pkT, i, "StartTime");
        int32 iEnd    = ShnGetI32(*pkT, i, "EndTime");
        int32 iMap    = ShnGetI32(*pkT, i, "MapID");
        bool  bActive = InWindow(uiHHMM, iStart, iEnd);
        std::map<uint32, bool>::iterator it = g_kRowActive.find(i);
        if (it == g_kRowActive.end()) {
            g_kRowActive[i] = bActive;
            if (!bActive) continue;          // first sight; emit only the spawn case
        } else if (it->second == bActive) {
            continue;                         // no transition
        } else {
            it->second = bActive;
        }
        PacketBuffer body;
        body.WriteU8(4);                      // kind = NPC schedule transition
        body.WriteU8(bActive ? 1 : 0);        // 1 = spawn, 0 = despawn
        body.WriteU32((uint32)iNpcID);
        body.WriteU16((uint16)iMap);
        WorldManagerServer::Get().BroadcastToZones(NC_INTER_BROADCAST_CMD,
                                                   body.Data(), body.Size());
        SHINELOG_INFO("NpcSchedule npc=%d %s on map=%d",
                      iNpcID, bActive ? "SPAWN" : "DESPAWN", iMap);
    }
}

} // namespace fiesta
