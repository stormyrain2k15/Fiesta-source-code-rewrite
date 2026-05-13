// Server/DataReader/SHN/ItemPackage.h
// Auto-generated: one-file-per-SHN split for ItemPackage.shn
#ifndef SHINE_DATAREADER_SHN_ITEMPACKAGE_H
#define SHINE_DATAREADER_SHN_ITEMPACKAGE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ItemPackageRow {
    uint16           uiHandle;
    std::string      kItemID;
    std::string      kContent;
    uint16           uiNumber;
};

class ItemPackageShn {
public:
    static ItemPackageShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemPackageRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemPackageRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ITEMPACKAGE_H
