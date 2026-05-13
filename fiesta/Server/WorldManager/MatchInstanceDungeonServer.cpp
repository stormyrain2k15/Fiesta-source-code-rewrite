// Server/WorldManager/MatchInstanceDungeonServer.cpp
// Cross-zone party-queueing for instance dungeons (MID). The CN client
// shipped a richer matchmaker; the NA2016 build keeps a single FIFO per
// instance id. Once enough players are queued the WM picks a host zone,
// allocates the instance via the per-zone instance manager, and emits the
// enter ACK.
//
// The queue is in-memory; players re-queue on WM restart.
#include "WMServices.h"
#include "WorldManagerServer.h"
#include "../Common/NETCOMMAND.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

MatchInstanceDungeonServer& MatchInstanceDungeonServer::Get() {
    static MatchInstanceDungeonServer s; return s;
}

// Min party size per instance. The full table is in MIDInfo.shn but we
// hardcode the floor so a queue of 1 still works during dev.
static const size_t kMinParty = 4;

bool MatchInstanceDungeonServer::Queue(CharID c, uint32 uiMID) {
    if (c == 0) return false;
    Cancel(c);
    m_kQueues[uiMID].push_back(c);
    SHINELOG_INFO("MID queue cid=%u mid=%u depth=%u",
                  c, uiMID, (uint32)m_kQueues[uiMID].size());
    return true;
}

bool MatchInstanceDungeonServer::Cancel(CharID c) {
    bool bRemoved = false;
    for (std::map<uint32, std::vector<CharID> >::iterator it = m_kQueues.begin();
         it != m_kQueues.end(); ++it) {
        for (size_t i = 0; i < it->second.size(); ) {
            if (it->second[i] == c) {
                it->second.erase(it->second.begin() + i);
                bRemoved = true;
            } else ++i;
        }
    }
    return bRemoved;
}

void MatchInstanceDungeonServer::Tick() {
    for (std::map<uint32, std::vector<CharID> >::iterator it = m_kQueues.begin();
         it != m_kQueues.end(); ++it) {
        if (it->second.size() < kMinParty) continue;
        uint32 uiMID = it->first;
        // Pull the first kMinParty members.
        PacketBuffer body;
        body.WriteU32(uiMID);
        body.WriteU8((uint8)kMinParty);
        for (size_t i = 0; i < kMinParty; ++i) body.WriteU32(it->second[i]);
        it->second.erase(it->second.begin(), it->second.begin() + kMinParty);

        WorldManagerServer::Get().BroadcastToZones(NC_MID_QUEUE_REQ,
                                                   body.Data(), body.Size());
        SHINELOG_INFO("MID assemble mid=%u party=%u", uiMID, (uint32)kMinParty);
    }
}

} // namespace shine
