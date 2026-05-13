// Server/DataReader/SHN/KQItem.h
// Auto-generated: one-file-per-SHN split for KQItem.shn
#ifndef SHINE_DATAREADER_SHN_KQITEM_H
#define SHINE_DATAREADER_SHN_KQITEM_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct KQItemRow {
    std::string      kItemIndex;
    uint16           uiMoveSpdRate;
    uint16           uiAbsoluteAttack;
    uint16           uiPickupLimit;
};

class KQItemShn {
public:
    static KQItemShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<KQItemRow>& Rows() const { return m_kRows; }
private:
    std::vector<KQItemRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_KQITEM_H
