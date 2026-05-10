// Server/DataReader/SHN/HolyPromiseReward.h
// Auto-generated: one-file-per-SHN split for HolyPromiseReward.shn
#ifndef FIESTA_DATAREADER_SHN_HOLYPROMISEREWARD_H
#define FIESTA_DATAREADER_SHN_HOLYPROMISEREWARD_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct HolyPromiseRewardRow {
    uint8            uiLevel;
    uint8            uiClass;
    std::string      kItemIndex;
    uint16           uiLot;
    uint8            uiUpgrade;
    uint32           uiIO_Str;
    uint32           uiIO_Con;
    uint32           uiIO_Dex;
    uint32           uiIO_Int;
    uint32           uiIO_Men;
    uint32           uiRes1;
    uint32           uiRes2;
    uint32           uiRes3;
};

class HolyPromiseRewardShn {
public:
    static HolyPromiseRewardShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<HolyPromiseRewardRow>& Rows() const { return m_kRows; }
private:
    std::vector<HolyPromiseRewardRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_HOLYPROMISEREWARD_H
