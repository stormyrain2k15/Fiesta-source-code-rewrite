// Server/DataReader/SHN/MoverUseSkill.h
// Auto-generated: one-file-per-SHN split for MoverUseSkill.shn
#ifndef FIESTA_DATAREADER_SHN_MOVERUSESKILL_H
#define FIESTA_DATAREADER_SHN_MOVERUSESKILL_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MoverUseSkillRow {
    std::string      kMoverIDX;
    std::string      kActiveSkillIDX;
};

class MoverUseSkillShn {
public:
    static MoverUseSkillShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MoverUseSkillRow>& Rows() const { return m_kRows; }
private:
    std::vector<MoverUseSkillRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOVERUSESKILL_H
