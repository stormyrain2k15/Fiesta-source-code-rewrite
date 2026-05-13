// Server/DataReader/SHN/ItemOptions.h
// Auto-generated: one-file-per-SHN split for ItemOptions.shn
#ifndef SHINE_DATAREADER_SHN_ITEMOPTIONS_H
#define SHINE_DATAREADER_SHN_ITEMOPTIONS_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ItemOptionsRow {
    uint16           uiOptionDegree;
    uint32           uiType;
    uint8            uiInterval;
    int16            iRate;
    int16            iUnkCol4;
    int16            iUnkCol5;
    int16            iUnkCol6;
    int16            iUnkCol7;
    int16            iUnkCol8;
    int16            iUnkCol9;
    int16            iUnkCol10;
    int16            iUnkCol11;
};

class ItemOptionsShn {
public:
    static ItemOptionsShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemOptionsRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemOptionsRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ITEMOPTIONS_H
