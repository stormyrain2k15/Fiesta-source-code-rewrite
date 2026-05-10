// Server/DataReader/SHN/GuildGradeData.h
// Auto-generated: one-file-per-SHN split for GuildGradeData.shn
#ifndef FIESTA_DATAREADER_SHN_GUILDGRADEDATA_H
#define FIESTA_DATAREADER_SHN_GUILDGRADEDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GuildGradeDataRow {
    uint8            uiType;
    uint32           uiNeedFame;
    uint16           uiMaxOfMember;
    int16            iMaxOfGradeMember;
    int16            iUnkCol4;
    int16            iUnkCol5;
    int16            iUnkCol6;
    int16            iUnkCol7;
    int16            iUnkCol8;
    int16            iUnkCol9;
};

class GuildGradeDataShn {
public:
    static GuildGradeDataShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildGradeDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildGradeDataRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GUILDGRADEDATA_H
