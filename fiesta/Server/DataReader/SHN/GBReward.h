// Server/DataReader/SHN/GBReward.h
// Auto-generated: one-file-per-SHN split for GBReward.shn
#ifndef SHINE_DATAREADER_SHN_GBREWARD_H
#define SHINE_DATAREADER_SHN_GBREWARD_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBRewardRow {
    uint32           uiGameType;
    uint32           uiRewardType;
    std::string      kItem_INX;
};

class GBRewardShn {
public:
    static GBRewardShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBRewardRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBRewardRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBREWARD_H
