// Server/DataReader/SHN/GuildGradeScoreData.h
// Auto-generated: one-file-per-SHN split for GuildGradeScoreData.shn
#ifndef FIESTA_DATAREADER_SHN_GUILDGRADESCOREDATA_H
#define FIESTA_DATAREADER_SHN_GUILDGRADESCOREDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GuildGradeScoreDataRow {
    int16            iGradeScore;
    int16            iUnkCol1;
    int16            iUnkCol2;
    int16            iUnkCol3;
    int16            iUnkCol4;
    int16            iUnkCol5;
    int16            iUnkCol6;
};

class GuildGradeScoreDataShn {
public:
    static GuildGradeScoreDataShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildGradeScoreDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildGradeScoreDataRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GUILDGRADESCOREDATA_H
