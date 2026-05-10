// Server/DataReader/SHN/MobAbStateDropSetting.h
// Auto-generated: one-file-per-SHN split for MobAbStateDropSetting.shn
#ifndef FIESTA_DATAREADER_SHN_MOBABSTATEDROPSETTING_H
#define FIESTA_DATAREADER_SHN_MOBABSTATEDROPSETTING_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobAbStateDropSettingRow {
    std::string      kMobInx;
    std::string      kABStateInx;
    uint32           uiDropType;
    uint8            uiMaxCount;
};

class MobAbStateDropSettingShn {
public:
    static MobAbStateDropSettingShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobAbStateDropSettingRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobAbStateDropSettingRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOBABSTATEDROPSETTING_H
