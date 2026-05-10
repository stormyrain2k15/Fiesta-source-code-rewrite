// Server/DataReader/SHN/KingdomQuest.h
// Auto-generated: one-file-per-SHN split for KingdomQuest.shn
#ifndef FIESTA_DATAREADER_SHN_KINGDOMQUEST_H
#define FIESTA_DATAREADER_SHN_KINGDOMQUEST_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct KingdomQuestRow {
    int16            iID;
    std::string      kTitle;
    uint16           uiLimitTime;
    uint8            uiST_Year;
    uint8            uiST_Month;
    uint8            uiST_Day;
    uint8            uiST_Hour;
    uint8            uiST_Minute;
    uint8            uiST_Second;
    uint16           uiStartWaitTime;
    uint8            uiNextStartMode;
    uint16           uiNextStartDeleyMin;
    uint8            uiRepeatMode;
    uint16           uiRepeatCount;
    uint8            uiMinLevel;
    uint8            uiMaxLevel;
    uint16           uiMinPlayers;
    uint16           uiMaxPlayers;
    uint8            uiPlayerRepeatMode;
    uint16           uiPlayerRepeatCount;
    uint8            uiPlayerRevivalMode;
    uint8            uiPlayerRevivalCount;
    uint16           uiDemandQuest;
    uint16           uiDemandItem;
    uint8            uiDemandMobKill;
    uint32           uiRewardIndex;
    int16            iMapLink;
    int16            iUnkCol27;
    int16            iUnkCol28;
    int16            iUnkCol29;
    std::string      kScriptLanguage;
    std::string      kInitValue;
    uint32           uiUseClass;
    int8             iDemandGender;
    int8             iUnkCol34;
};

class KingdomQuestShn {
public:
    static KingdomQuestShn& Get();
    void Load();
    const KingdomQuestRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<KingdomQuestRow>& Rows() const { return m_kRows; }
private:
    std::vector<KingdomQuestRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_KINGDOMQUEST_H
