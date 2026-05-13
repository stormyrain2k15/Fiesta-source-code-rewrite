// Server/DataReader/SHN/DamageLvGapEVP.h
// Auto-generated: one-file-per-SHN split for DamageLvGapEVP.shn
#ifndef SHINE_DATAREADER_SHN_DAMAGELVGAPEVP_H
#define SHINE_DATAREADER_SHN_DAMAGELVGAPEVP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

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

} // namespace shine
#endif // SHINE_DATAREADER_SHN_DAMAGELVGAPEVP_H
