// Server/Zone/Tables/RandomOptionTable.cpp
// FEATURE: world-creation -- World/RandomOptionTable.txt binder.
// Per-drop random-option recipe: how many sockets to roll, the min/max
// range for each of Str/Con/Dex/Int/Men, and whether the option is
// hidden until the item is identified.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"

namespace fiesta {

RandomOptionTable& RandomOptionTable::Get() { static RandomOptionTable s; return s; }

bool RandomOptionTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\RandomOptionTable.txt")) return false;
    const TsTable* t = f.Find("RandomOptionTable"); if (!t) return false;
    static const char* kMin[5] = {"StrMin","ConMin","DexMin","IntMin","MenMin"};
    static const char* kMax[5] = {"StrMax","ConMax","DexMax","IntMax","MenMax"};
    // FEATURE: world-creation -- column read: DropItemIndex, OptionHide,
    // MinOpCount, MaxOpCount, StrMin/Max, ConMin/Max, DexMin/Max,
    // IntMin/Max, MenMin/Max
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        RandomOptionRow x;
        x.kDropItemIndex = t->GetStr(r, "DropItemIndex");
        x.uiOptionHide   = (uint8)t->GetInt(r, "OptionHide");
        x.uiMinOpCount   = (uint8)t->GetInt(r, "MinOpCount");
        x.uiMaxOpCount   = (uint8)t->GetInt(r, "MaxOpCount");
        for (int k = 0; k < 5; ++k) {
            x.aMin[k] = (uint16)t->GetInt(r, kMin[k]);
            x.aMax[k] = (uint16)t->GetInt(r, kMax[k]);
        }
        if (!x.kDropItemIndex.empty()) m_kRows[x.kDropItemIndex] = x;
    }
    return true;
}

const RandomOptionRow* RandomOptionTable::Find(const std::string& r) const {
    std::map<std::string, RandomOptionRow>::const_iterator it = m_kRows.find(r);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

} // namespace fiesta
