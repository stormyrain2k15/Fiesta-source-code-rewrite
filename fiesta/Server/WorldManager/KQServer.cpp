// Server/WorldManager/KQServer.cpp
// Kingdom Quest authority. KQs are world-global events: per-kingdom voting
// is open for a window, then KQ::Start closes voting and spawns the per-zone
// instance with the winning kingdom theme. Players join/leave on the WM so
// the assignment survives zone transfers; reward distribution happens when
// the per-zone PineScript emits Finish() back to the WM.
#include "WMServices.h"
#include "WorldManagerServer.h"
#include "WMCharDBClient.h"
#include "../Common/NETCOMMAND.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

KQServer& KQServer::Get() { static KQServer s; return s; }

static const uint64 kKQDurationMs = 30ULL * 60ULL * 1000ULL;   // 30-min wave window

void KQServer::Vote(uint32 uiKQID, uint32 uiKingdom) {
    // Find or insert the in-flight instance.
    Inst* pkInst = NULL;
    for (size_t i = 0; i < m_kRunning.size(); ++i) {
        if (m_kRunning[i].uiKQID == uiKQID) { pkInst = &m_kRunning[i]; break; }
    }
    if (!pkInst) {
        Inst k; k.uiKQID = uiKQID; k.uiEndsAtMs = 0; k.bRunning = false;
        m_kRunning.push_back(k);
        pkInst = &m_kRunning.back();
    }
    if (pkInst->bRunning) return;     // voting closed once started
    ++pkInst->kVotes[uiKingdom];
}

void KQServer::Start(uint32 uiKQID) {
    for (size_t i = 0; i < m_kRunning.size(); ++i) {
        Inst& k = m_kRunning[i];
        if (k.uiKQID != uiKQID) continue;
        if (k.bRunning) return;
        k.bRunning  = true;
        k.uiEndsAtMs = GTimer::NowMillis() + kKQDurationMs;
        // Pick winning kingdom (highest votes).
        uint32 uiWinKingdom = 0; uint32 uiBest = 0;
        for (std::map<uint32, uint32>::iterator it = k.kVotes.begin();
             it != k.kVotes.end(); ++it) {
            if (it->second > uiBest) { uiBest = it->second; uiWinKingdom = it->first; }
        }
        PacketBuffer body;
        body.WriteU32(uiKQID);
        body.WriteU32(uiWinKingdom);
        WorldManagerServer::Get().BroadcastToZones(NC_KQ_STATE_CMD,
                                                   body.Data(), body.Size());
        SHINELOG_INFO("KQ #%u START winKingdom=%u", uiKQID, uiWinKingdom);
        return;
    }
}

void KQServer::Join(uint32 uiKQID, CharID c) {
    for (size_t i = 0; i < m_kRunning.size(); ++i) {
        if (m_kRunning[i].uiKQID == uiKQID) {
            m_kRunning[i].kPlayers.insert(c);
            return;
        }
    }
}

void KQServer::Leave(uint32 uiKQID, CharID c) {
    for (size_t i = 0; i < m_kRunning.size(); ++i) {
        if (m_kRunning[i].uiKQID == uiKQID) {
            m_kRunning[i].kPlayers.erase(c);
            return;
        }
    }
}

void KQServer::Finish(uint32 uiKQID, uint32 uiWinningKingdom) {
    for (size_t i = 0; i < m_kRunning.size(); ++i) {
        if (m_kRunning[i].uiKQID != uiKQID) continue;
        WMCharDBClient::Get().KQResultSet(uiKQID, uiWinningKingdom);

        PacketBuffer body;
        body.WriteU32(uiKQID);
        body.WriteU32(uiWinningKingdom);
        body.WriteU16((uint16)m_kRunning[i].kPlayers.size());
        for (std::set<CharID>::iterator it = m_kRunning[i].kPlayers.begin();
             it != m_kRunning[i].kPlayers.end(); ++it) {
            body.WriteU32(*it);
        }
        WorldManagerServer::Get().BroadcastToZones(NC_KQ_ENTER_CMD,
                                                   body.Data(), body.Size());
        SHINELOG_INFO("KQ #%u FINISH winKingdom=%u players=%u",
                      uiKQID, uiWinningKingdom, (uint32)m_kRunning[i].kPlayers.size());
        m_kRunning.erase(m_kRunning.begin() + i);
        return;
    }
}

void KQServer::Tick() {
    uint64 uiNow = GTimer::NowMillis();
    for (size_t i = 0; i < m_kRunning.size(); ) {
        Inst& k = m_kRunning[i];
        if (k.bRunning && uiNow >= k.uiEndsAtMs) {
            // Auto-finish: pick winner by participant kingdom plurality vote.
            uint32 uiWin = 0; uint32 uiBest = 0;
            for (std::map<uint32, uint32>::iterator it = k.kVotes.begin();
                 it != k.kVotes.end(); ++it) {
                if (it->second > uiBest) { uiBest = it->second; uiWin = it->first; }
            }
            uint32 uiKQID = k.uiKQID;
            Finish(uiKQID, uiWin);
            // Finish() erased the entry; restart loop without ++.
        } else ++i;
    }
}

} // namespace shine
