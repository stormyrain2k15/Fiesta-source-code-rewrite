// Server/DataReader/SHN/RaceNameInfo.h
// Auto-generated: one-file-per-SHN split for RaceNameInfo.shn
#ifndef SHINE_DATAREADER_SHN_RACENAMEINFO_H
#define SHINE_DATAREADER_SHN_RACENAMEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct RaceNameInfoRow {
    uint8            uiRaceID;
    std::string      kAcPrefix;
    std::string      kAcEngName;
    std::string      kAcLocalName;
};

class RaceNameInfoShn {
public:
    static RaceNameInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<RaceNameInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<RaceNameInfoRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_RACENAMEINFO_H
