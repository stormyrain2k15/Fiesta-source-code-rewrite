// Server/DataReader/SHN/MobConditionServer.h
// Auto-generated: one-file-per-SHN split for MobConditionServer.shn
#ifndef FIESTA_DATAREADER_SHN_MOBCONDITIONSERVER_H
#define FIESTA_DATAREADER_SHN_MOBCONDITIONSERVER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobConditionServerRow {
    std::string      kMobInx;
    uint8            uiAniLv;
    uint32           uiMC_Type;
    uint32           uiMC_ValueMin;
    uint32           uiMC_ValueMax;
};

class MobConditionServerShn {
public:
    static MobConditionServerShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobConditionServerRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobConditionServerRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOBCONDITIONSERVER_H
