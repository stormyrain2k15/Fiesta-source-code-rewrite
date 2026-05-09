// Server/Zone/GuildAcademy.h
// Guild academy: a per-guild buff aura attached to the academy NPC.
// Loaded from GuildAcademy.shn -- the row carries the buff InxName,
// the minimum membership time before a member qualifies, and the
// rank-aggregation flag (whether sub-guilds inherit).
#ifndef FIESTA_ZONE_GUILDACADEMY_H
#define FIESTA_ZONE_GUILDACADEMY_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <map>

namespace fiesta {

class ShinePlayer;

class GuildAcademy {
public:
    static GuildAcademy& Get();
    bool Load();

    // Apply the academy buff to a member who is currently at the
    // academy NPC. Returns false when the member is unknown or has
    // not yet served the minimum join time. Buff is applied via the
    // standard AbnormalState pipeline.
    bool TryApplyBuff(ShinePlayer* pkPlayer);

    // Drop the academy buff (e.g. when the player leaves the academy
    // proximity, or when they disconnect / leave the guild).
    void DropBuff(ShinePlayer* pkPlayer);

    // Per-guild count of members currently inside the academy (used
    // by guild-leader UI to surface "X members training").
    uint32 ActiveMembers(uint32 uiGuildID) const;
private:
    GuildAcademy() : m_uiLeastJoinSec(0), m_bRankAggregation(false) {}

    std::string m_kBuffName;          // "StaAcaGenalBuf" by default
    uint32      m_uiLeastJoinSec;     // 60 s in the stock SHN
    bool        m_bRankAggregation;
    // active per-guild members keyed by (guildID -> set of charIDs)
    std::map<uint32, std::map<uint32, uint32> > m_kActive;
};

} // namespace fiesta
#endif
