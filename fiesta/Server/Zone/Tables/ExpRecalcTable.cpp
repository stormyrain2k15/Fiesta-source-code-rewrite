// Server/Zone/Tables/ExpRecalcTable.cpp
// FEATURE: world-creation -- World/ExpRecalculation.txt binder.
// Two sub-tables: ByPartyMem (party-size XP bonus) and ByLevelDiff
// (kill-vs-target level XP scalar). Bonuses are stored x1000.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ExpRecalcTable& ExpRecalcTable::Get() { static ExpRecalcTable s; return s; }

bool ExpRecalcTable::Load(const std::string& rRoot) {
    for (int i = 0; i < 8; ++i) m_aPartyBonus[i] = 1000;
    m_kLevelDiffBonus.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\ExpRecalculation.txt")) return false;

    // FEATURE: world-creation -- column read: PartyMember, Bonus
    if (const TsTable* t = f.Find("ByPartyMem")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint8 m = (uint8)t->GetInt(r, "PartyMember");
            int32 b = (int32)t->GetInt(r, "Bonus");
            if (m < 8) m_aPartyBonus[m] = b;
        }
    }
    // FEATURE: world-creation -- column read: LevelDiff, Bonus
    if (const TsTable* t = f.Find("ByLevelDiff")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            int32 d = (int32)t->GetInt(r, "LevelDiff");
            int32 b = (int32)t->GetInt(r, "Bonus");
            m_kLevelDiffBonus[d] = b;
        }
    }
    return true;
}

int32 ExpRecalcTable::PartyBonus(uint8 m) const {
    return (m < 8) ? m_aPartyBonus[m] : 1000;
}
int32 ExpRecalcTable::LevelDiffBonus(int32 d) const {
    std::map<int32, int32>::const_iterator it = m_kLevelDiffBonus.find(d);
    if (it != m_kLevelDiffBonus.end()) return it->second;
    // Find nearest below, since the file uses sparse anchor rows.
    int32 best = 1000; bool found = false;
    for (it = m_kLevelDiffBonus.begin(); it != m_kLevelDiffBonus.end(); ++it) {
        if (it->first <= d) { best = it->second; found = true; }
        else if (!found) { best = it->second; }
    }
    return best;
}
int32 ExpRecalcTable::Scaler(uint16 uiKillerLv, uint16 uiMobLv) const {
    int32 diff = (int32)uiKillerLv - (int32)uiMobLv;
    int32 bonus = LevelDiffBonus(diff);     // x1000
    return bonus / 10;                       // -> percentage
}

} // namespace shine
