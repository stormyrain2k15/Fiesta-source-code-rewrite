// Server/DataReader/SHN/CharacterTitleData.h
// Per-SHN split for CharacterTitleData.shn
#ifndef SHINE_DATAREADER_SHN_CHARACTERTITLEDATA_H
#define SHINE_DATAREADER_SHN_CHARACTERTITLEDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct CharacterTitleDataRow {
    uint32      uiType;
    uint32      uiPermit;
    uint32      uiRefresh;
    std::string kTitle0;  uint32 uiValue0; uint32 uiFame0;
    std::string kTitle1;  uint32 uiValue1; uint32 uiFame1;
    std::string kTitle2;  uint32 uiValue2; uint32 uiFame2;
    std::string kTitle3;  uint32 uiValue3; uint32 uiFame3;
};

class CharacterTitleDataShn {
public:
    static CharacterTitleDataShn& Get();
    void Load();
    const CharacterTitleDataRow* FindByType(uint32 uiType) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<CharacterTitleDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<CharacterTitleDataRow>   m_kRows;
    std::map<uint32, size_t>             m_kByType;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_CHARACTERTITLEDATA_H
