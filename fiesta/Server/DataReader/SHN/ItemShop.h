// Server/DataReader/SHN/ItemShop.h
// Auto-generated: one-file-per-SHN split for ItemShop.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMSHOP_H
#define FIESTA_DATAREADER_SHN_ITEMSHOP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemShopRow {
    uint32           uiGoodsNo;
    std::string      kInxName;
    uint32           uiLot;
};

class ItemShopShn {
public:
    static ItemShopShn& Get();
    void Load();
    const ItemShopRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemShopRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemShopRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMSHOP_H
