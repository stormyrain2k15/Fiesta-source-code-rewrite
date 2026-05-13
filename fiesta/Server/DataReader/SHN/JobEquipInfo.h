// Server/DataReader/SHN/JobEquipInfo.h
// Auto-generated: one-file-per-SHN split for JobEquipInfo.shn
#ifndef SHINE_DATAREADER_SHN_JOBEQUIPINFO_H
#define SHINE_DATAREADER_SHN_JOBEQUIPINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct JobEquipInfoRow {
    uint32           uiChrClass;
    std::string      kEqu_RightHand;
    std::string      kEqu_LeftHand;
    std::string      kEqu_Shoes;
    std::string      kEqu_Head;
    std::string      kEqu_Leg;
    std::string      kEqu_Body;
    std::string      kEqu_ETC;
};

class JobEquipInfoShn {
public:
    static JobEquipInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<JobEquipInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<JobEquipInfoRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_JOBEQUIPINFO_H
