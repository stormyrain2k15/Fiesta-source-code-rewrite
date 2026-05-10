// Server/DataReader/SHN/PupFactorCondition.h
// Auto-generated: one-file-per-SHN split for PupFactorCondition.shn
#ifndef FIESTA_DATAREADER_SHN_PUPFACTORCONDITION_H
#define FIESTA_DATAREADER_SHN_PUPFACTORCONDITION_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct PupFactorConditionRow {
    uint32           uiPupMindType;
    uint32           uiPupFactorConditionType;
    uint32           uiPupFactorType;
    uint8            uiIsMinus;
    uint32           uiValue;
};

class PupFactorConditionShn {
public:
    static PupFactorConditionShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PupFactorConditionRow>& Rows() const { return m_kRows; }
private:
    std::vector<PupFactorConditionRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_PUPFACTORCONDITION_H
