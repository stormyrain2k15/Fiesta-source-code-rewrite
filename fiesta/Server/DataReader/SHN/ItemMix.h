// Server/DataReader/SHN/ItemMix.h
// Auto-generated: one-file-per-SHN split for ItemMix.shn
#ifndef SHINE_DATAREADER_SHN_ITEMMIX_H
#define SHINE_DATAREADER_SHN_ITEMMIX_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

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

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ITEMMIX_H
