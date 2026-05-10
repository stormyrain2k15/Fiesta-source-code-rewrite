// Server/DataReader/SHN/ItemSort.h
// Auto-generated: one-file-per-SHN split for ItemSort.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMSORT_H
#define FIESTA_DATAREADER_SHN_ITEMSORT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemSortRow {
    std::string      kItemSort_Index;
};

class ItemSortShn {
public:
    static ItemSortShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemSortRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemSortRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMSORT_H
