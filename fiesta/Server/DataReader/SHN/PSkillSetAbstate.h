// Server/DataReader/SHN/PSkillSetAbstate.h
// Auto-generated: one-file-per-SHN split for PSkillSetAbstate.shn
#ifndef FIESTA_DATAREADER_SHN_PSKILLSETABSTATE_H
#define FIESTA_DATAREADER_SHN_PSKILLSETABSTATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct PSkillSetAbstateRow {
    std::string      kPS_InxName;
    uint32           uiPS_Condition;
    uint16           uiPS_ConditioRate;
    std::string      kPS_AbStateInx;
    uint8            uiStrength;
};

class PSkillSetAbstateShn {
public:
    static PSkillSetAbstateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PSkillSetAbstateRow>& Rows() const { return m_kRows; }
private:
    std::vector<PSkillSetAbstateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_PSKILLSETABSTATE_H
