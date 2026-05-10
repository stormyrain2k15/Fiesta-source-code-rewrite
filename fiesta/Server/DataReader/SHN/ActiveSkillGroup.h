// Server/DataReader/SHN/ActiveSkillGroup.h
// Auto-generated: one-file-per-SHN split for ActiveSkillGroup.shn
#ifndef FIESTA_DATAREADER_SHN_ACTIVESKILLGROUP_H
#define FIESTA_DATAREADER_SHN_ACTIVESKILLGROUP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ActiveSkillGroupRow {
    std::string      kInxName;
    uint32           uiActiveSkillGroupIndex;
};

class ActiveSkillGroupShn {
public:
    static ActiveSkillGroupShn& Get();
    void Load();
    const ActiveSkillGroupRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ActiveSkillGroupRow>& Rows() const { return m_kRows; }
private:
    std::vector<ActiveSkillGroupRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ACTIVESKILLGROUP_H
