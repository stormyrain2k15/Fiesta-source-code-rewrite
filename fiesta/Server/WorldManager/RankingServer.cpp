// Server/WorldManager/RankingServer.cpp
// Cross-zone leaderboard. Each zone publishes its sorted-top-N list to WM
// every minute; RankingServer merges the per-zone slices into a single
// global ranking and republishes to CharDB once an hour for persistence.
//
// Categories (mirrored from RankingTable.shn):
//   0 = level / fame   1 = guild fame      2 = guild war wins
//   3 = MID clears     4 = arena (PvP)     5 = production / craft
//
// Hourly publish goes through WMCharDBClient::RankingPublish ->
// p_Ranking_Publish on the CharDB exe. Per-request reads serve a page of
// the in-memory copy without hitting the DB.
#include "WMServices.h"
#include "WMCharDBClient.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

RankingServer& RankingServer::Get() { static RankingServer s; return s; }

static const uint64 kPublishIntervalMs = 60ULL * 60ULL * 1000ULL;   // 1 hour
static const size_t kMaxCategories     = 8;

// In-memory category -> snapshot. Each snapshot is a flat list of DBRecord
// rows (cols: rank, charNo, charName, score). The hourly Tick() pushes a
// copy of every populated category to CharDB.
static std::vector<DBRecord> g_kCats[kMaxCategories];

void RankingServer::Tick() {
    uint64 uiNow = GTimer::NowMillis();
    if (m_uiNextPublishMs == 0) m_uiNextPublishMs = uiNow + kPublishIntervalMs;
    if (uiNow < m_uiNextPublishMs) return;
    m_uiNextPublishMs = uiNow + kPublishIntervalMs;
    for (uint8 c = 0; c < kMaxCategories; ++c) {
        if (g_kCats[c].empty()) continue;
        WMCharDBClient::Get().RankingPublish(c, g_kCats[c]);
        SHINELOG_INFO("Ranking publish cat=%u rows=%u",
                      (uint32)c, (uint32)g_kCats[c].size());
    }
}

void RankingServer::RequestList(uint8 uiCategory, std::vector<DBRecord>& rOut) {
    rOut.clear();
    if (uiCategory >= kMaxCategories) return;
    rOut = g_kCats[uiCategory];
}

// Called by the WM-side dispatcher when a zone uploads its slice. Cross-
// zone merge: we replace the per-category list wholesale (the zones each
// hold their own segment of the ranking and the most-recent upload wins).
void RankingServer_OnZoneSlice(uint8 uiCategory, const std::vector<DBRecord>& rRows) {
    if (uiCategory >= kMaxCategories) return;
    g_kCats[uiCategory] = rRows;
}

} // namespace shine
