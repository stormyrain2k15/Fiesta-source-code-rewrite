// Server/Zone/Tables/ItemDropTableW.h
// FEATURE: world-creation -- World/ItemDropTable.txt declarations.
// Per-(map, mob) drop matrix: 45 numbered drop slots + 5 exclusive
// items per row. Each slot has its own item, drop rate (R, per-1000),
// upgrade range, rule code (r/m/...), and quantity.
#ifndef SHINE_ZONE_TABLES_ITEMDROPTABLEW_H
#define SHINE_ZONE_TABLES_ITEMDROPTABLEW_H
#include "../../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ItemDropSlot {
    std::string kItem;          // "-" if empty
    uint32      uiRate;         // per-1000000 (1M scale per NA2016 file)
    uint16      uiUpgradeMin;
    uint16      uiUpgradeMax;
    char        cRule;          // 'r' = random, 'm' = always min, ...
    int32       iNum;
};

struct ItemDropMobRow {
    std::string  kMapArea;       // "-" = any
    std::string  kMobId;
    uint16       uiMinLevel;
    uint16       uiMaxLevel;
    uint8        uiAbStateCnt;
    uint32       uiMinCen;
    uint32       uiMaxCen;
    uint32       uiCenRate;
    std::string  kTradeBoxA, kTradeBoxB, kTradeBoxC;
    uint32       uiRateA, uiRateB, uiRateC;
    ItemDropSlot aSlots[45];
    std::string  aExcItem[5];
};

class ItemDropTableW {
public:
    static ItemDropTableW& Get();
    bool Load(const std::string& rRoot);
    // Look up a mob's drop row. Mob is keyed on inx-name; the map filter
    // is applied by callers when they have a current map context.
    void RowsForMob(const std::string& rMobInx,
                    std::vector<const ItemDropMobRow*>& rOut) const;
    size_t Count() const { return m_kRows.size(); }
private:
    ItemDropTableW() {}
    std::vector<ItemDropMobRow> m_kRows;
    std::map<std::string, std::vector<size_t> > m_kByMob;
};

} // namespace shine
#endif
