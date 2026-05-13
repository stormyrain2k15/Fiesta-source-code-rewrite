// Server/DataReader/SHN/CollectCardTitle.h
// Auto-generated: one-file-per-SHN split for CollectCardTitle.shn
#ifndef SHINE_DATAREADER_SHN_COLLECTCARDTITLE_H
#define SHINE_DATAREADER_SHN_COLLECTCARDTITLE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct CollectCardTitleRow {
    uint32           uiType;
    std::string      kCC_ItemInx;
};

class CollectCardTitleShn {
public:
    static CollectCardTitleShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<CollectCardTitleRow>& Rows() const { return m_kRows; }
private:
    std::vector<CollectCardTitleRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_COLLECTCARDTITLE_H
