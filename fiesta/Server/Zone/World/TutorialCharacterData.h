// Server/Zone/World/TutorialCharacterData.h
// Auto-generated: loader for TutorialCharacterData.txt (Shine root).
#ifndef SHINE_ZONE_WORLD_TUTORIALCHARACTERDATA_H
#define SHINE_ZONE_WORLD_TUTORIALCHARACTERDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <string>
#include <vector>

namespace shine {

struct TutorialCharacterDataRow {
    int32            iClass;
    std::string      kStartMapName;
    int32            iStartPX;
    int32            iStartPY;
    int32            iHP;
    int32            iSP;
    int32            iHPSoulStoneCount;
    int32            iSPSoulStoneCount;
    int32            iMoney;
};

class TutorialCharacterDataTable {
public:
    static TutorialCharacterDataTable& Get();
    void Load(const std::string& rPath);
    const std::vector<TutorialCharacterDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<TutorialCharacterDataRow> m_kRows;
};

} // namespace shine
#endif // SHINE_ZONE_WORLD_TUTORIALCHARACTERDATA_H
