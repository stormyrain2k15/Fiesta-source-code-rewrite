// Server/Zone/Tables/MobChatTable.cpp
// FEATURE: world-creation -- World/MobChat.txt binder.
// Six chat-line buckets per mob: PIECE, ATTACK, DAMAGED, DEAD,
// HELPMAIN, HELPSUB. Each row holds 4 candidate scripts + per-1000
// rates for the first two; the rest fan out across the remaining
// quartile. Picks one stochastically when a mob fires the trigger.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include <stdlib.h>

namespace shine {

MobChatTable& MobChatTable::Get() { static MobChatTable s; return s; }

bool MobChatTable::Load(const std::string& rRoot) {
    for (int b = 0; b < MC_BUCKETS; ++b) { m_kBucket[b].clear(); m_kIndex[b].clear(); }
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\MobChat.txt")) return false;
    static const char* kNames[MC_BUCKETS] = {
        "PIECE", "ATTACK", "DAMAGED", "DEAD", "HELPMAIN", "HELPSUB"
    };
    // FEATURE: world-creation -- column read: MobIndex, Rate0, Rate1,
    // Script0..Script3
    for (int b = 0; b < MC_BUCKETS; ++b) {
        const TsTable* t = f.Find(kNames[b]); if (!t) continue;
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            Row x;
            x.kMobIndex = t->GetStr(r, "MobIndex");
            x.uiRate0   = (uint32)t->GetInt(r, "Rate0");
            x.uiRate1   = (uint32)t->GetInt(r, "Rate1");
            x.aScript[0]= t->GetStr(r, "Script0");
            x.aScript[1]= t->GetStr(r, "Script1");
            x.aScript[2]= t->GetStr(r, "Script2");
            x.aScript[3]= t->GetStr(r, "Script3");
            m_kIndex[b][x.kMobIndex].push_back(m_kBucket[b].size());
            m_kBucket[b].push_back(x);
        }
    }
    return true;
}

std::string MobChatTable::Pick(eMobChatBucket eB, const std::string& rM) const {
    if ((int)eB < 0 || (int)eB >= MC_BUCKETS) return std::string();
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kIndex[eB].find(rM);
    if (it == m_kIndex[eB].end()) return std::string();
    if (it->second.empty()) return std::string();
    const Row& r = m_kBucket[eB][it->second[0]];
    uint32 dice = (uint32)(rand() % 100);
    if (dice < r.uiRate0)                         return r.aScript[0];
    if (dice < r.uiRate0 + r.uiRate1)             return r.aScript[1];
    if (dice < r.uiRate0 + r.uiRate1 + 50)        return r.aScript[2];
    return r.aScript[3];
}

} // namespace shine
