// Server/DataReader/SHN/PartyBonusByLvDiff.h
// Auto-generated: one-file-per-SHN split for PartyBonusByLvDiff.shn
#ifndef FIESTA_DATAREADER_SHN_PARTYBONUSBYLVDIFF_H
#define FIESTA_DATAREADER_SHN_PARTYBONUSBYLVDIFF_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct PartyBonusByLvDiffRow {
    uint16           uiPB_LvDiff;
    uint16           uiPB_BonusRatio;
};

class PartyBonusByLvDiffShn {
public:
    static PartyBonusByLvDiffShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PartyBonusByLvDiffRow>& Rows() const { return m_kRows; }
private:
    std::vector<PartyBonusByLvDiffRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_PARTYBONUSBYLVDIFF_H
