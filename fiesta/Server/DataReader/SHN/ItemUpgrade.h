// Server/DataReader/SHN/ItemUpgrade.h
// Auto-generated: one-file-per-SHN split for ItemUpgrade.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMUPGRADE_H
#define FIESTA_DATAREADER_SHN_ITEMUPGRADE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemUpgradeRow {
    uint32           uiID;
    uint16           uiCriFail;
    uint16           uiDownFail;
    uint16           uiNormalFail;
    uint16           uiNCon;
    uint16           uiLuckySuc;
};

class ItemUpgradeShn {
public:
    static ItemUpgradeShn& Get();
    void Load();
    const ItemUpgradeRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemUpgradeRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemUpgradeRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMUPGRADE_H
