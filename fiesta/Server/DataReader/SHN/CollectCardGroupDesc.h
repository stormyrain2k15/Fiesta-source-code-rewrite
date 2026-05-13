// Server/DataReader/SHN/CollectCardGroupDesc.h
// Auto-generated: one-file-per-SHN split for CollectCardGroupDesc.shn
#ifndef SHINE_DATAREADER_SHN_COLLECTCARDGROUPDESC_H
#define SHINE_DATAREADER_SHN_COLLECTCARDGROUPDESC_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct CollectCardGroupDescRow {
    uint32           uiCC_CardMobGroup;
    std::string      kCC_MobGroupName;
};

class CollectCardGroupDescShn {
public:
    static CollectCardGroupDescShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<CollectCardGroupDescRow>& Rows() const { return m_kRows; }
private:
    std::vector<CollectCardGroupDescRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_COLLECTCARDGROUPDESC_H
