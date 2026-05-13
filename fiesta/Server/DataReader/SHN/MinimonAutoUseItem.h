// Server/DataReader/SHN/MinimonAutoUseItem.h
// Auto-generated: one-file-per-SHN split for MinimonAutoUseItem.shn
#ifndef SHINE_DATAREADER_SHN_MINIMONAUTOUSEITEM_H
#define SHINE_DATAREADER_SHN_MINIMONAUTOUSEITEM_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MinimonAutoUseItemRow {
    uint32           uiMAUItem;
    std::string      kItemIDX;
};

class MinimonAutoUseItemShn {
public:
    static MinimonAutoUseItemShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MinimonAutoUseItemRow>& Rows() const { return m_kRows; }
private:
    std::vector<MinimonAutoUseItemRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MINIMONAUTOUSEITEM_H
