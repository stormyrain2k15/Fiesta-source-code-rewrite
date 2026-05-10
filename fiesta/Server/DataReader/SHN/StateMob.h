// Server/DataReader/SHN/StateMob.h
// Auto-generated: one-file-per-SHN split for StateMob.shn
#ifndef FIESTA_DATAREADER_SHN_STATEMOB_H
#define FIESTA_DATAREADER_SHN_STATEMOB_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct StateMobRow {
    std::string      kTargetMobInx;
    std::string      kConditionMobInx;
    std::string      kAbStateInx;
    uint32           uiStrength;
};

class StateMobShn {
public:
    static StateMobShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<StateMobRow>& Rows() const { return m_kRows; }
private:
    std::vector<StateMobRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_STATEMOB_H
