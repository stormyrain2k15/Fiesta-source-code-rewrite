// Server/Zone/GuildAcademy.cpp
// Guild academy: real impl backed by GuildAcademy.shn. The row carries
// the buff InxName, the minimum join-time threshold (seconds), and the
// rank-aggregation flag. TryApplyBuff() resolves the buff via
// AbnormalStateDictionary and pushes a runtime row into the player's
// AbState ledger; the buff persists until the player leaves academy
// proximity or disconnects.
#include "GuildAcademy.h"
#include "ShineObject.h"
#include "AbState.h"
#include "AbnormalStateDictionary.h"
#include "GuildSystem.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/GTimer.h"

namespace shine {

GuildAcademy& GuildAcademy::Get() { static GuildAcademy s; return s; }

bool GuildAcademy::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildAcademy");
    if (!t || t->RecordCount() == 0) {
        SHINELOG_WARN("GuildAcademy: GuildAcademy.shn missing or empty");
        return false;
    }
    // The stock data drop carries a single row; we read row 0 and
    // accept any extra rows as overrides for higher tiers (later rows
    // supersede earlier ones with matching keys -- last-write-wins).
    for (uint32 i = 0; i < t->RecordCount(); ++i) {
        m_kBuffName        = ShnGetStr(*t, i, "BuffName");
        m_uiLeastJoinSec   = ShnGetU32(*t, i, "LeastJoinTime");
        m_bRankAggregation = ShnGetU32(*t, i, "RankAggregation") != 0;
    }
    SHINELOG_INFO("GuildAcademy loaded: buff='%s' joinTime=%us aggregate=%d",
                  m_kBuffName.c_str(), m_uiLeastJoinSec,
                  m_bRankAggregation ? 1 : 0);
    return true;
}

bool GuildAcademy::TryApplyBuff(ShinePlayer* pkPlayer) {
    if (!pkPlayer || m_kBuffName.empty()) return false;
    // Resolve the player's guild + join timestamp through GuildServer.
    uint32 uiGuild    = GuildServer::Get().GuildOf (pkPlayer->GetCharID());
    uint64 uiJoinedMs = GuildServer::Get().JoinedMs(pkPlayer->GetCharID());
    if (uiGuild == 0) return false;
    uint64 now        = GTimer::NowMillis();
    if (uiJoinedMs == 0 || (now - uiJoinedMs) < (uint64)m_uiLeastJoinSec * 1000ULL) {
        SHINELOG_DEBUG("GuildAcademy: cid=%u below join-time threshold",
                       pkPlayer->GetCharID());
        return false;
    }
    uint32 uiAb = AbnormalStateDictionary::Get().Lookup(m_kBuffName);
    if (uiAb == 0) {
        SHINELOG_WARN("GuildAcademy: buff '%s' not in dictionary",
                      m_kBuffName.c_str());
        return false;
    }
    // Long keep cap; OnLeaveProximity / disconnect calls DropBuff.
    if (!pkPlayer->AbState().Apply(uiAb, 60 * 60 * 1000, 1))
        return false;
    m_kActive[uiGuild][pkPlayer->GetCharID()] = (uint32)now;
    return true;
}

void GuildAcademy::DropBuff(ShinePlayer* pkPlayer) {
    if (!pkPlayer || m_kBuffName.empty()) return;
    uint32 uiGuild = GuildServer::Get().GuildOf(pkPlayer->GetCharID());
    uint32 uiAb = AbnormalStateDictionary::Get().Lookup(m_kBuffName);
    if (uiAb != 0) pkPlayer->AbState().Remove(uiAb);
    if (uiGuild != 0) {
        std::map<uint32, std::map<uint32, uint32> >::iterator git =
            m_kActive.find(uiGuild);
        if (git != m_kActive.end()) git->second.erase(pkPlayer->GetCharID());
    }
}

uint32 GuildAcademy::ActiveMembers(uint32 uiGuildID) const {
    std::map<uint32, std::map<uint32, uint32> >::const_iterator it =
        m_kActive.find(uiGuildID);
    return (it == m_kActive.end()) ? 0 : (uint32)it->second.size();
}

} // namespace shine
