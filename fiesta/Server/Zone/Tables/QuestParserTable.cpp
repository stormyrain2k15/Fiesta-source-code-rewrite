// Server/Zone/Tables/QuestParserTable.cpp
// FEATURE: world-creation -- World/QuestParser.txt binder.
// Format is a flat token-pair list, not a #TABLE TableScriptFile, so
// we parse it directly.
#include "QuestParserTable.h"
#include "../../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <stdlib.h>

namespace fiesta {

QuestParserTable& QuestParserTable::Get() { static QuestParserTable s; return s; }

bool QuestParserTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    std::string path = rRoot + "\\World\\QuestParser.txt";
    FILE* f = NULL;
#if defined(_MSC_VER)
    fopen_s(&f, path.c_str(), "rb");
#else
    f = fopen(path.c_str(), "rb");
#endif
    if (!f) {
        SHINELOG_WARN("QuestParser.txt missing");
        return false;
    }
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char name[64]; int id = 0;
        if (sscanf_s(line, "%63s %d", name, (unsigned)sizeof(name), &id) == 2 && name[0]) {
            m_kRows[name] = id;
        }
    }
    fclose(f);
    SHINELOG_INFO("QuestParser.txt: %u class tokens", (uint32)m_kRows.size());
    return true;
}

int32 QuestParserTable::ClassIdFor(const std::string& rN) const {
    std::map<std::string, int32>::const_iterator it = m_kRows.find(rN);
    return (it == m_kRows.end()) ? -1 : it->second;
}

} // namespace fiesta
