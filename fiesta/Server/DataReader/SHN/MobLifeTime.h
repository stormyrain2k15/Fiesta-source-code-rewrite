// Server/DataReader/SHN/MobLifeTime.h
// Auto-generated: one-file-per-SHN split for MobLifeTime.shn
#ifndef FIESTA_DATAREADER_SHN_MOBLIFETIME_H
#define FIESTA_DATAREADER_SHN_MOBLIFETIME_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobLifeTimeRow {
    std::string      kMobIndex;
    uint16           uiLifeTime;
};

class MobLifeTimeShn {
public:
    static MobLifeTimeShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobLifeTimeRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobLifeTimeRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOBLIFETIME_H
