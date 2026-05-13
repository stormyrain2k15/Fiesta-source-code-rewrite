// Server/DataReader/SHN/ItemMerchantInfo.h
// Auto-generated: one-file-per-SHN split for ItemMerchantInfo.shn
#ifndef SHINE_DATAREADER_SHN_ITEMMERCHANTINFO_H
#define SHINE_DATAREADER_SHN_ITEMMERCHANTINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ItemMerchantInfoRow {
    uint8            uiMap;
    uint8            uiSub;
};

class ItemMerchantInfoShn {
public:
    static ItemMerchantInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemMerchantInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemMerchantInfoRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ITEMMERCHANTINFO_H
