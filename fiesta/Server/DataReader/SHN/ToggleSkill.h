// Server/DataReader/SHN/ToggleSkill.h
// Auto-generated: one-file-per-SHN split for ToggleSkill.shn
#ifndef FIESTA_DATAREADER_SHN_TOGGLESKILL_H
#define FIESTA_DATAREADER_SHN_TOGGLESKILL_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ToggleSkillRow {
    std::string      kTS_SkillInx;
    uint32           uiTS_Condition;
    uint16           uiTS_Value;
};

class ToggleSkillShn {
public:
    static ToggleSkillShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ToggleSkillRow>& Rows() const { return m_kRows; }
private:
    std::vector<ToggleSkillRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_TOGGLESKILL_H
