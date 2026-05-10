// Server/DataReader/SHN/BRAccUpgrade.h
// Auto-generated: one-file-per-SHN split for BRAccUpgrade.shn
#ifndef FIESTA_DATAREADER_SHN_BRACCUPGRADE_H
#define FIESTA_DATAREADER_SHN_BRACCUPGRADE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct BRAccUpgradeRow {
    uint32           uiID;
    uint16           uiCriFail;
    uint16           uiDownFail;
    uint16           uiNormalFail;
    uint16           uiNCon;
    uint16           uiLuckySuc;
};

class BRAccUpgradeShn {
public:
    static BRAccUpgradeShn& Get();
    void Load();
    const BRAccUpgradeRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<BRAccUpgradeRow>& Rows() const { return m_kRows; }
private:
    std::vector<BRAccUpgradeRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_BRACCUPGRADE_H
