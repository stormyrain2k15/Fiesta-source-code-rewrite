// Server/Zone/Tables/ItemOptionsTable.cpp
// FEATURE: world-creation -- World/ItemOptions.txt binder.
#include "ItemOptionsTable.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"
#include <stdio.h>

namespace fiesta {

ItemOptionsTable& ItemOptionsTable::Get() { static ItemOptionsTable s; return s; }

std::string ItemOptionsTable::Key(uint16 d, const std::string& t) {
    char buf[40]; sprintf_s(buf, sizeof(buf), "%u|", (unsigned)d);
    return std::string(buf) + t;
}

bool ItemOptionsTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\ItemOptions.txt")) return false;
    const TsTable* t = f.Find("Options"); if (!t) return false;
    // FEATURE: world-creation -- column read: OptionDegree, type, 1..9
    static const char* kCols[10] = {
        "1","2","3","4","5","6","7","8","9", "10"
    };
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        LegacyItemOptionsRow x;
        x.uiOptionDegree = (uint16)t->GetInt(r, "OptionDegree");
        x.kType          = t->GetStr(r, "type");
        if (x.kType.empty()) continue;
        for (int k = 0; k < 10; ++k)
            x.aWeights[k] = (uint16)t->GetInt(r, kCols[k]);
        m_kRows[Key(x.uiOptionDegree, x.kType)] = x;
    }
    SHINELOG_INFO("ItemOptions.txt: %u (degree,type) curves", (uint32)m_kRows.size());
    return true;
}

int32 ItemOptionsTable::PickBucket(uint16 uiDegree, const std::string& rType,
                                   uint32 uiRoll) const {
    std::map<std::string, LegacyItemOptionsRow>::const_iterator it =
        m_kRows.find(Key(uiDegree, rType));
    if (it == m_kRows.end()) return -1;
    uint32 acc = 0;
    for (int k = 0; k < 10; ++k) {
        acc += it->second.aWeights[k];
        if (uiRoll < acc) return k;
    }
    return -1;
}

} // namespace fiesta
