// Server/DataReader/SHN/KingdomQuestRew.h
// Auto-generated: one-file-per-SHN split for KingdomQuestRew.shn
#ifndef SHINE_DATAREADER_SHN_KINGDOMQUESTREW_H
#define SHINE_DATAREADER_SHN_KINGDOMQUESTREW_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct KingdomQuestRewRow {
    uint32           uiID;
    std::string      kIndexString;
    std::string      kKQBoxItemIDX;
    int16            iReward;
    int16            iUnkCol4;
    int16            iUnkCol5;
    int16            iUnkCol6;
    int16            iUnkCol7;
    int16            iUnkCol8;
    int16            iUnkCol9;
    int16            iUnkCol10;
    int16            iUnkCol11;
    int16            iUnkCol12;
    int16            iUnkCol13;
    int16            iUnkCol14;
    int16            iUnkCol15;
    int16            iUnkCol16;
    int16            iUnkCol17;
    int16            iRewardRate;
    int16            iUnkCol19;
    int16            iUnkCol20;
    int16            iUnkCol21;
    int16            iUnkCol22;
    int16            iUnkCol23;
    int16            iUnkCol24;
    int16            iUnkCol25;
    int16            iUnkCol26;
    int16            iUnkCol27;
    int16            iUnkCol28;
    int16            iUnkCol29;
    int16            iUnkCol30;
    int16            iUnkCol31;
    int16            iUnkCol32;
};

class KingdomQuestRewShn {
public:
    static KingdomQuestRewShn& Get();
    void Load();
    const KingdomQuestRewRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<KingdomQuestRewRow>& Rows() const { return m_kRows; }
private:
    std::vector<KingdomQuestRewRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_KINGDOMQUESTREW_H
