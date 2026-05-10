// Server/Zone/Tables/QuestParserTable.h
// FEATURE: world-creation -- World/QuestParser.txt declarations.
// Plain (className, classID) lookup used by quest-script parsing
// to resolve class-name tokens into the canonical class id. The file
// is a flat list ("Fighter 1", "CleverFighter 2", ...).
#ifndef FIESTA_ZONE_TABLES_QUESTPARSERTABLE_H
#define FIESTA_ZONE_TABLES_QUESTPARSERTABLE_H
#include "../../Shared/ShineTypes.h"
#include <map>
#include <string>

namespace fiesta {

class QuestParserTable {
public:
    static QuestParserTable& Get();
    bool Load(const std::string& rRoot);
    int32 ClassIdFor(const std::string& rClassName) const;
    size_t Count() const { return m_kRows.size(); }
private:
    QuestParserTable() {}
    std::map<std::string, int32> m_kRows;
};

} // namespace fiesta
#endif
