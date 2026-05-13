// Server/DataReader/SHN/WeaponTitleData.h
// Auto-generated: one-file-per-SHN split for WeaponTitleData.shn
#ifndef SHINE_DATAREADER_SHN_WEAPONTITLEDATA_H
#define SHINE_DATAREADER_SHN_WEAPONTITLEDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct WeaponTitleDataRow {
    uint16           uiMobID;
    uint8            uiLevel;
    std::string      kPrefix;
    std::string      kSuffix;
    uint32           uiMobKillCount;
    uint16           uiMinAdd;
    uint16           uiMaxAdd;
    uint8            uiSP1_Reference;
    int16            iSP1_Type;
    uint32           uiSP1_Value;
    uint8            uiSP2_Reference;
    int16            iSP2_Type;
    uint32           uiSP2_Value;
    uint8            uiSP3_Reference;
    int16            iSP3_Type;
    uint32           uiSP3_Value;
};

class WeaponTitleDataShn {
public:
    static WeaponTitleDataShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<WeaponTitleDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<WeaponTitleDataRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_WEAPONTITLEDATA_H
