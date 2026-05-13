// Server/Zone/AttackRhythm.h
// Per-(charId, skillId) attack/skill cadence gate. Used by Skill::TryUse
// and normal-attack handlers to enforce a minimum interval between
// consecutive uses of the same skill -- the original NA2016 server
// rejects fast-fire packets at this layer rather than relying on
// client-side cooldown.
#ifndef SHINE_ZONE_ATTACKRHYTHM_H
#define SHINE_ZONE_ATTACKRHYTHM_H
#include "../Shared/ShineTypes.h"
#include <map>

namespace shine {

class AttackRhythm {
public:
    static AttackRhythm& Get();
    // Returns true if `cid` may use `sid` now; advances the last-use
    // timestamp on success. `cdMs` is the minimum interval; pass 0 for
    // "no cadence gate" (skill-cooldown table handles it instead).
    bool Allow(uint32 cid, uint32 sid, uint32 cdMs);
    // Drop a player's whole rhythm history on disconnect.
    void Forget(uint32 cid);
private:
    AttackRhythm() {}
    std::map<uint64, uint64> m_kLast;   // key = (cid<<32) | sid -> ms
};

} // namespace shine
#endif
