// Server/DataReader/SHN/GTWinScore.h
// Auto-generated: one-file-per-SHN split for GTWinScore.shn
#ifndef SHINE_DATAREADER_SHN_GTWINSCORE_H
#define SHINE_DATAREADER_SHN_GTWINSCORE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GTWinScoreRow {
    uint16           uiWinScore;
};

class GTWinScoreShn {
public:
    static GTWinScoreShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GTWinScoreRow>& Rows() const { return m_kRows; }
private:
    std::vector<GTWinScoreRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GTWINSCORE_H
