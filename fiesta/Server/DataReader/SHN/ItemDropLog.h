// Server/DataReader/SHN/ItemDropLog.h
// Auto-generated: one-file-per-SHN split for ItemDropLog.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMDROPLOG_H
#define FIESTA_DATAREADER_SHN_ITEMDROPLOG_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemDropLogRow {
    uint32           uiItemID;
};

class ItemDropLogShn {
public:
    static ItemDropLogShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemDropLogRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemDropLogRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMDROPLOG_H
