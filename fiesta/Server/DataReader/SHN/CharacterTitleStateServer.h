// Server/DataReader/SHN/CharacterTitleStateServer.h
// Auto-generated: one-file-per-SHN split for CharacterTitleStateServer.shn
#ifndef FIESTA_DATAREADER_SHN_CHARACTERTITLESTATESERVER_H
#define FIESTA_DATAREADER_SHN_CHARACTERTITLESTATESERVER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct CharacterTitleStateServerRow {
    uint32           uiType;
    uint8            uiTitleLV;
    std::string      kStateName;
    uint8            uiStrength;
};

class CharacterTitleStateServerShn {
public:
    static CharacterTitleStateServerShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<CharacterTitleStateServerRow>& Rows() const { return m_kRows; }
private:
    std::vector<CharacterTitleStateServerRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_CHARACTERTITLESTATESERVER_H
