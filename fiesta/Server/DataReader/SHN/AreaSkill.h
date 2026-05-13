// Server/DataReader/SHN/AreaSkill.h
// Auto-generated: one-file-per-SHN split for AreaSkill.shn
#ifndef SHINE_DATAREADER_SHN_AREASKILL_H
#define SHINE_DATAREADER_SHN_AREASKILL_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct AreaSkillRow {
    std::string      kAS_SkillInx;
    uint8            uiAS_Step;
    std::string      kAS_BMPIndex;
    uint32           uiAS_ImagePin;
    uint8            uiAS_IsDirection;
};

class AreaSkillShn {
public:
    static AreaSkillShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<AreaSkillRow>& Rows() const { return m_kRows; }
private:
    std::vector<AreaSkillRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_AREASKILL_H
