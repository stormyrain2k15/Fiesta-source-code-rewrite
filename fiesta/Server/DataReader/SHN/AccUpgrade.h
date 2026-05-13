// Server/DataReader/SHN/AccUpgrade.h
// Auto-generated: one-file-per-SHN split for AccUpgrade.shn
#ifndef SHINE_DATAREADER_SHN_ACCUPGRADE_H
#define SHINE_DATAREADER_SHN_ACCUPGRADE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct AccUpgradeRow {
    uint32           uiID;
    uint16           uiCriFail;
    uint16           uiDownFail;
    uint16           uiNormalFail;
    uint16           uiNCon;
    uint16           uiLuckySuc;
};

class AccUpgradeShn {
public:
    static AccUpgradeShn& Get();
    void Load();
    const AccUpgradeRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<AccUpgradeRow>& Rows() const { return m_kRows; }
private:
    std::vector<AccUpgradeRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ACCUPGRADE_H
