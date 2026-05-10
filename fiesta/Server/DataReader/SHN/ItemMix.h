// Server/DataReader/SHN/ItemMix.h
// Auto-generated: one-file-per-SHN split for ItemMix.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMMIX_H
#define FIESTA_DATAREADER_SHN_ITEMMIX_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemMixRow {
    std::string      kRaw_ItemIDX;
    std::string      kMix_ItemIDX;
};

class ItemMixShn {
public:
    static ItemMixShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemMixRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemMixRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMMIX_H
