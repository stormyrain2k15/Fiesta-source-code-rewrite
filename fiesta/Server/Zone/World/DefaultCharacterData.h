// Server/Zone/World/DefaultCharacterData.h
// Auto-generated: loader for DefaultCharacterData.txt (Shine root).
#ifndef SHINE_ZONE_WORLD_DEFAULTCHARACTERDATA_H
#define SHINE_ZONE_WORLD_DEFAULTCHARACTERDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <string>
#include <vector>

namespace shine {

struct DefaultCharacterDataRow {
    int32            iClass;
    std::string      kStartMapName;
    int32            iStartPX;
    int32            iStartPY;
    int32            iHP;
    int32            iSP;
    int32            iHPSoulStoneCount;
    int32            iSPSoulStoneCount;
    int32            iMoney;
    int32            iInitLV;
    std::string      kInitEXP;
};

class DefaultCharacterDataTable {
public:
    static DefaultCharacterDataTable& Get();
    void Load(const std::string& rPath);
    const std::vector<DefaultCharacterDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<DefaultCharacterDataRow> m_kRows;
};

} // namespace shine
#endif // SHINE_ZONE_WORLD_DEFAULTCHARACTERDATA_H
