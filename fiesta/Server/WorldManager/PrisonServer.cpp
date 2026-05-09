// Server/WorldManager/PrisonServer.cpp
// PK-offense tracking and prison-sentence management. Each PK kill records
// 60 sentence-seconds against the killer; once accumulated time is non-zero
// the next zone-enter will route the player to the prison map. Sentences
// decrement once per minute via Tick(), and the per-zone hosting the
// prisoner is notified every decrement so the client timer stays in sync.
#include "WMServices.h"
#include "WorldManagerServer.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

PrisonServer& PrisonServer::Get() { static PrisonServer s; return s; }

static const uint32 kSecondsPerKill   = 60;
static const uint32 kMaxSentenceSecs  = 60 * 60 * 4;     // 4-hour cap
static const uint64 kDecrementMs      = 60ULL * 1000ULL; // 1 min

void PrisonServer::RecordPK(CharID killer, CharID /*victim*/) {
    if (killer == 0) return;
    uint32& uiSecs = m_kSentenceSecs[killer];
    uint32 uiNew = uiSecs + kSecondsPerKill;
    if (uiNew > kMaxSentenceSecs) uiNew = kMaxSentenceSecs;
    uiSecs = uiNew;
    SHINELOG_INFO("Prison sentence cid=%u total=%u sec", killer, uiSecs);

    PacketBuffer body; body.WriteU32(killer); body.WriteU32(uiSecs);
    WorldManagerServer::Get().BroadcastToZones(NC_PRISON_ENTER_CMD,
                                               body.Data(), body.Size());
}

bool PrisonServer::IsPrisoner(CharID c, uint32* pSecsLeft) const {
    std::map<CharID, uint32>::const_iterator it = m_kSentenceSecs.find(c);
    if (it == m_kSentenceSecs.end() || it->second == 0) {
        if (pSecsLeft) *pSecsLeft = 0;
        return false;
    }
    if (pSecsLeft) *pSecsLeft = it->second;
    return true;
}

void PrisonServer::Tick() {
    uint64 uiNow = GTimer::NowMillis();
    if (m_uiNextDecMs == 0) m_uiNextDecMs = uiNow + kDecrementMs;
    if (uiNow < m_uiNextDecMs) return;
    m_uiNextDecMs = uiNow + kDecrementMs;

    for (std::map<CharID, uint32>::iterator it = m_kSentenceSecs.begin();
         it != m_kSentenceSecs.end(); ) {
        if (it->second <= 60) {
            CharID c = it->first;
            std::map<CharID, uint32>::iterator del = it++;
            m_kSentenceSecs.erase(del);
            PacketBuffer body; body.WriteU32(c);
            WorldManagerServer::Get().BroadcastToZones(NC_PRISON_LEAVE_CMD,
                                                       body.Data(), body.Size());
        } else {
            it->second -= 60;
            ++it;
        }
    }
}

} // namespace fiesta
