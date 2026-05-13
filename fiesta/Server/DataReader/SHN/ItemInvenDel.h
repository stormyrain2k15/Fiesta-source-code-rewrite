// Server/DataReader/SHN/ItemInvenDel.h
// Auto-generated: one-file-per-SHN split for ItemInvenDel.shn
#ifndef SHINE_DATAREADER_SHN_ITEMINVENDEL_H
#define SHINE_DATAREADER_SHN_ITEMINVENDEL_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ItemInvenDelRow {
    std::string      kInxName;
    uint32           uiDelCondition;
};

class ItemInvenDelShn {
public:
    static ItemInvenDelShn& Get();
    void Load();
    const ItemInvenDelRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemInvenDelRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemInvenDelRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ITEMINVENDEL_H
