// Server/DataReader/SHN/DamageLvGapPVE.h
// Auto-generated: one-file-per-SHN split for DamageLvGapPVE.shn
#ifndef FIESTA_DATAREADER_SHN_DAMAGELVGAPPVE_H
#define FIESTA_DATAREADER_SHN_DAMAGELVGAPPVE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct DamageLvGapPVERow {
    uint16           uiLvGap;
    uint16           uiDamageRate;
};

class DamageLvGapPVEShn {
public:
    static DamageLvGapPVEShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<DamageLvGapPVERow>& Rows() const { return m_kRows; }
private:
    std::vector<DamageLvGapPVERow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_DAMAGELVGAPPVE_H
