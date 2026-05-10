// Server/DataReader/SHN/DamageLvGapEVP.h
// Auto-generated: one-file-per-SHN split for DamageLvGapEVP.shn
#ifndef FIESTA_DATAREADER_SHN_DAMAGELVGAPEVP_H
#define FIESTA_DATAREADER_SHN_DAMAGELVGAPEVP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct DamageLvGapEVPRow {
    uint16           uiLvGap;
    uint16           uiDamageRate;
};

class DamageLvGapEVPShn {
public:
    static DamageLvGapEVPShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<DamageLvGapEVPRow>& Rows() const { return m_kRows; }
private:
    std::vector<DamageLvGapEVPRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_DAMAGELVGAPEVP_H
