// Server/Zone/DeathReviveSystem.h
// the on-death revive dialog flow.
// When a character dies the client shows a dialog with:
//   * "Use Shine Soul Stone"  -- consumes one Shine Soul (a separate counter
//                                from HP/SP soul stones); restores 100% HP/SP,
//                                no XP penalty, revive in-place.
//   * "Town Respawn"          -- free; warp to bind town; XP penalty per
//                                ExpLossOnTownRevive(level).
//   * "Wait for Resurrect"    -- another player may cast a resurrect skill.
#ifndef SHINE_ZONE_DEATHREVIVESYSTEM_H
#define SHINE_ZONE_DEATHREVIVESYSTEM_H
#include "../Shared/ShineTypes.h"

namespace shine {

class ShinePlayer;

struct ShineSoulPouch {
    uint16 uiCount;     // shine soul stones held (revive currency)

    ShineSoulPouch() : uiCount(0) {}
};

enum eReviveChoice {
    REVIVE_NONE      = 0,
    REVIVE_SHINE     = 1,    // consume 1 Shine Soul, full HP/SP, in-place
    REVIVE_TOWN      = 2,    // free, warp to town, XP penalty
    REVIVE_RESURRECT = 3     // accept a queued resurrect skill from a healer
};

enum eReviveResult {
    REV_OK              = 0,
    REV_NO_SHINE        = 1,
    REV_NOT_DEAD        = 2,
    REV_NO_RESURRECT    = 3,    // no queued resurrect skill available
    REV_INVALID         = 4
};

class DeathReviveSystem {
public:
    static eReviveResult Choose(ShinePlayer*    pkP,
                                ShineSoulPouch& rPouch,
                                eReviveChoice   eChoice);

    // XP penalty per-mille for town respawn (placeholder curve; tune in-place).
    static int32 ExpLossOnTownRevive(int32 nLevel);

    // Award a Shine Soul stone (KQ reward, daily login, cash shop).
    static void GrantShineSoul(ShineSoulPouch& rPouch, uint16 uiQty);
};

} // namespace shine
#endif
