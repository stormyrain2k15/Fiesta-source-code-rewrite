// Server/Zone/MultiHitTable.h
// Public surface for the multi-hit data table. The .cpp owns the load
// against MultiHitType.shn and the per-group lookup. SkillSystem.cpp
// references `MultiHitTable::Resolve(SkillID)` for the hit-count
// branch in `Skill::TryUse`.
#ifndef SHINE_ZONE_MULTIHITTABLE_H
#define SHINE_ZONE_MULTIHITTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>

namespace shine {

struct MultiHitEntry {
    uint8  bGroupID;
    uint16 wHitDelayMs;
    uint16 wDamageRateX1k;
};

class MultiHitTable {
public:
    static MultiHitTable& Get();
    bool                  Load();

    // Returns a pointer to the contiguous entries for `bGroupID` and
    // writes the count to *pCount. Returns NULL when the group isn't
    // present (caller must treat as a single-hit skill).
    const MultiHitEntry*  GetGroup(uint8 bGroupID, int* pCount) const;

    // SkillSystem hot-path entry. Returns the number of hits the skill
    // emits. Returns 1 for skills with no multi-hit row (the common
    // case).
    static int32          Resolve(SkillID sid);

private:
    MultiHitTable() {}
    std::map<uint8, std::vector<MultiHitEntry> > m_kByGroup;
};

} // namespace shine
#endif
