// Server/Zone/Tables/ItemOptionsTable.h
// FEATURE: world-creation -- World/ItemOptions.txt declarations.
// Per-(OptionDegree, type) weight ladder of 9 buckets. The runtime
// rolls a random uniform weight against the cumulative ladder to pick
// the option count for that stat at that degree tier.
#ifndef FIESTA_ZONE_TABLES_ITEMOPTIONSTABLE_H
#define FIESTA_ZONE_TABLES_ITEMOPTIONSTABLE_H
#include "../../Shared/ShineTypes.h"
#include <map>
#include <string>

namespace fiesta {

struct LegacyItemOptionsRow {
    uint16 uiOptionDegree;
    std::string kType;
    uint16 aWeights[10];        // weights[0..9], cumulative sum precomputed
};

class ItemOptionsTable {
public:
    static ItemOptionsTable& Get();
    bool Load(const std::string& rRoot);
    // Pick a bucket index 0..9 for (degree, type) given a uniform roll
    // in [0..total-1]; returns -1 on miss.
    int32 PickBucket(uint16 uiDegree, const std::string& rType,
                     uint32 uiRoll) const;
    size_t Count() const { return m_kRows.size(); }
private:
    ItemOptionsTable() {}
    // key: (degree << 16) ^ stable hash of type
    std::map<std::string, LegacyItemOptionsRow> m_kRows;
    static std::string Key(uint16 d, const std::string& t);
};

} // namespace fiesta
#endif
