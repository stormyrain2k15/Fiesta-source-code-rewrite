// Server/DataReader/SHN/ItemMoney.h
// Auto-generated: one-file-per-SHN split for ItemMoney.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMMONEY_H
#define FIESTA_DATAREADER_SHN_ITEMMONEY_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemMoneyRow {
    std::string      kIM_ItemIDX;
    std::string      kIM_MoneyIDX;
    uint16           uiIM_MoneyLot;
};

class ItemMoneyShn {
public:
    static ItemMoneyShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemMoneyRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemMoneyRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMMONEY_H
