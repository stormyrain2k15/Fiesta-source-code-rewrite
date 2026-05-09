// Server/Zone/AggroList.h
// 12 -- per-mob aggro / hate accumulator + level-gap scaler.
// EVIDENCE: PDB_CONFIRMED  symbol: AggroList, HateList, ThreatTable
//
// Maintains a (charId, hate) priority list per mob. Damage / heal-on-tank /
// taunt skills push hate via Add(). The mob AI consults Top() each tick to
// pick its target.
//
// Level-gap scaling
// -----------------
// Original game rule: aggro scales over a **20-level span** centred on the
// mob's level.
//
//   * player.level <= mob.level                        -> full aggro (1.0x)
//   * player.level >  mob.level && < mob.level + 20    -> linearly fades
//   * player.level >= mob.level + 20                   -> no aggro (0.0x)
//
// The closer the player is to (or below) the mob's level the harder the
// mob "notices" them; the further above the mob the player is the less
// the mob cares. The scaler is applied at hate-push time so a passing
// high-level player walking through low-level mobs doesn't pull aggro.
//
// Detect / chase ranges (from MobInfoServer.shn DetectCha / FollowCha) are
// consulted by MobAI before any hate is pushed; AggroList itself is
// purely the threat-priority queue.
#ifndef FIESTA_ZONE_AGGROLIST_H
#define FIESTA_ZONE_AGGROLIST_H
#include "../Shared/ShineTypes.h"
#include <vector>

namespace fiesta {

const int32 kAggroLevelSpan = 20;

class AggroList {
public:
    struct Entry { CharID c; int32 iHate; };

    AggroList() {}

    void Add   (CharID c, int32 iAmount);  // accumulates
    void Set   (CharID c, int32 iAmount);  // overwrite
    void Drop  (CharID c);
    void Clear ()                          { m_kEntries.clear(); }
    bool Empty () const                    { return m_kEntries.empty(); }

    // Highest-hate target right now (returns 0 if empty).
    CharID Top () const;

    // Decay by `iAmount` on every entry; entries that hit 0 are removed.
    // Called by mob AI tick at a low cadence (e.g. once per 5s) so combat
    // taunts naturally fade after the fight.
    void Decay(int32 iAmount);

    const std::vector<Entry>& Entries() const { return m_kEntries; }

    // -----------------------------------------------------------------
    // Level-gap scaler. Returns the threat multiplier (x1000 fixed-point)
    // that should be applied to a hate amount before pushing it onto the
    // list, given the attacker's level vs the mob's level.
    //   uiPlayerLevel <= uiMobLevel                : 1000  (1.000x)
    //   uiPlayerLevel >= uiMobLevel + kSpan        : 0
    //   in between                                  : linear fade
    // -----------------------------------------------------------------
    static int32 LevelGapMulX1k(uint16 uiPlayerLevel, uint16 uiMobLevel);

    // Convenience: pre-scaled hate push.
    void AddScaled(CharID c, int32 iRawHate, uint16 uiPlayerLevel, uint16 uiMobLevel);
private:
    std::vector<Entry> m_kEntries;
    Entry* Find(CharID c);
};

} // namespace fiesta
#endif
