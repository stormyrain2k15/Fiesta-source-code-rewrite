// Server/WorldManager/PartyFinderServer.cpp
// Cross-zone party-finder board. Per-player party state lives on the Zone
// that hosts the leader; the WM merely maintains the global "looking for
// party" board so a player on Zone3 can see and join a post created by a
// player on Zone1. State is pure in-memory -- the board is intentionally
// volatile across WM restarts.
#include "WMServices.h"
#include "../Shared/GTimer.h"
#include "../DataServer/Common/Database.h"

namespace fiesta {

PartyFinderServer& PartyFinderServer::Get() { static PartyFinderServer s; return s; }

// Posts older than this are auto-pruned on each Get / Tick.
static const uint64 kPostTtlMs = 10ULL * 60ULL * 1000ULL;

bool PartyFinderServer::Post(CharID host, uint8 uiCategory, const std::string& rNote) {
    if (host == 0) return false;
    Cancel(host);                                  // overwrite previous post
    Listing p; p.host = host; p.cat = uiCategory; p.note = rNote;
    p.uiAt = GTimer::NowMillis();
    m_kPosts.push_back(p);
    return true;
}

bool PartyFinderServer::Cancel(CharID host) {
    for (size_t i = 0; i < m_kPosts.size(); ++i) {
        if (m_kPosts[i].host == host) {
            m_kPosts.erase(m_kPosts.begin() + i);
            return true;
        }
    }
    return false;
}

void PartyFinderServer::GetList(uint8 uiCategory, std::vector<DBRecord>& rOut) const {
    uint64 uiNow = GTimer::NowMillis();
    rOut.clear();
    for (size_t i = 0; i < m_kPosts.size(); ++i) {
        if (uiNow - m_kPosts[i].uiAt > kPostTtlMs) continue;
        if (uiCategory != 0xFF && m_kPosts[i].cat != uiCategory) continue;
        DBRecord r;
        char buf[32];
        _snprintf(buf, sizeof(buf), "%u", m_kPosts[i].host); r.kCols.push_back(buf);
        _snprintf(buf, sizeof(buf), "%u", (uint32)m_kPosts[i].cat); r.kCols.push_back(buf);
        r.kCols.push_back(m_kPosts[i].note);
        rOut.push_back(r);
    }
}

void PartyFinderServer::Tick() {
    uint64 uiNow = GTimer::NowMillis();
    for (size_t i = 0; i < m_kPosts.size(); ) {
        if (uiNow - m_kPosts[i].uiAt > kPostTtlMs)
            m_kPosts.erase(m_kPosts.begin() + i);
        else ++i;
    }
}

} // namespace fiesta
