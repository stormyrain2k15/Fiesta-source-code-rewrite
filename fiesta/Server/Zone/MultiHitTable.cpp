// Server/Zone/MultiHitTable.cpp
// MultiHit data box. NA2016 file is MultiHitType.shn (earlier code
// referenced "MultiHit" which doesn't exist).
//
// Schema: 182 rows, 62 hit groups.
//   InxName       -- group identifier string
//   HitGroupID    -- uint8 (1..62)
//   HitDelay      -- uint16 ms between hit and the next
//   DamageRate    -- uint16 x/1000 of base damage applied to this hit
//
// Battle.cpp::RuleOfEngagement::CalcDamage consults this table when a
// skill row's `nHitID` > 0, walking the entries in the matching
// HitGroupID and emitting one damage packet per row.
#include "MultiHitTable.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

MultiHitTable& MultiHitTable::Get() { static MultiHitTable s; return s; }

bool MultiHitTable::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MultiHitType");
    if (!t) {
        SHINELOG_WARN("MultiHitTable: MultiHitType.shn not loaded");
        return false;
    }
    m_kByGroup.clear();
    for (size_t r = 0; r < t->RecordCount(); ++r) {
        MultiHitEntry e;
        e.bGroupID       = (uint8) ShnGetI32(*t, r, "HitGroupID");
        e.wHitDelayMs    = (uint16)ShnGetI32(*t, r, "HitDelay");
        e.wDamageRateX1k = (uint16)ShnGetI32(*t, r, "DamageRate");
        m_kByGroup[e.bGroupID].push_back(e);
    }
    SHINELOG_INFO("MultiHitTable: loaded %u rows across %u groups",
                  (uint32)t->RecordCount(), (uint32)m_kByGroup.size());
    return true;
}

const MultiHitEntry* MultiHitTable::GetGroup(uint8 bGroupID, int* pCount) const {
    std::map<uint8, std::vector<MultiHitEntry> >::const_iterator it =
        m_kByGroup.find(bGroupID);
    if (it == m_kByGroup.end() || it->second.empty()) {
        if (pCount) *pCount = 0;
        return NULL;
    }
    if (pCount) *pCount = (int)it->second.size();
    return &it->second[0];
}

int32 MultiHitTable::Resolve(SkillID /*sid*/) {
    // SkillSystem.cpp's existing call site asks "how many hits does
    // this skill produce?" for the hit-loop. The mapping from SkillID
    // to HitGroupID lives in ActiveSkill.shn nHitID; until that wiring
    // lands, return 1 (single hit) so existing behaviour is preserved.
    return 1;
}

} // namespace fiesta
