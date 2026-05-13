// Server/DataReader/SHN/KQTeam.h
// Auto-generated: one-file-per-SHN split for KQTeam.shn
#ifndef SHINE_DATAREADER_SHN_KQTEAM_H
#define SHINE_DATAREADER_SHN_KQTEAM_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct KQTeamRow {
    int16            iID;
    uint8            uiMaxMemberGap;
    uint8            uiIsTeamPVP;
    int16            iKQTeamDivideType;
    uint32           uiRegenXRed;
    uint32           uiRegenYRed;
    uint32           uiRegenXBlue;
    uint32           uiRegenYBlue;
};

class KQTeamShn {
public:
    static KQTeamShn& Get();
    void Load();
    const KQTeamRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<KQTeamRow>& Rows() const { return m_kRows; }
private:
    std::vector<KQTeamRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_KQTEAM_H
