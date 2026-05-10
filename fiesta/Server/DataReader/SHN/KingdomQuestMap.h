// Server/DataReader/SHN/KingdomQuestMap.h
// Auto-generated: one-file-per-SHN split for KingdomQuestMap.shn
#ifndef FIESTA_DATAREADER_SHN_KINGDOMQUESTMAP_H
#define FIESTA_DATAREADER_SHN_KINGDOMQUESTMAP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct KingdomQuestMapRow {
    uint8            uiNumOfMap;
    std::string      kBaseMap;
    std::string      kMap;
    std::string      kUnkCol3;
    std::string      kUnkCol4;
    std::string      kUnkCol5;
    std::string      kUnkCol6;
    std::string      kUnkCol7;
    std::string      kUnkCol8;
    std::string      kUnkCol9;
    std::string      kUnkCol10;
    std::string      kUnkCol11;
    int8             iClear;
    int8             iUnkCol13;
    int8             iUnkCol14;
    int8             iUnkCol15;
    int8             iUnkCol16;
    int8             iUnkCol17;
    int8             iUnkCol18;
    int8             iUnkCol19;
    int8             iUnkCol20;
    int8             iUnkCol21;
};

class KingdomQuestMapShn {
public:
    static KingdomQuestMapShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<KingdomQuestMapRow>& Rows() const { return m_kRows; }
private:
    std::vector<KingdomQuestMapRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_KINGDOMQUESTMAP_H
