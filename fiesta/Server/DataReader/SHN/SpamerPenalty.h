// Server/DataReader/SHN/SpamerPenalty.h
// Auto-generated: one-file-per-SHN split for SpamerPenalty.shn
#ifndef FIESTA_DATAREADER_SHN_SPAMERPENALTY_H
#define FIESTA_DATAREADER_SHN_SPAMERPENALTY_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct SpamerPenaltyRow {
    uint8            uiPenaltyLv;
    uint16           uiChatBlockTime;
    uint16           uiProbateTime;
};

class SpamerPenaltyShn {
public:
    static SpamerPenaltyShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<SpamerPenaltyRow>& Rows() const { return m_kRows; }
private:
    std::vector<SpamerPenaltyRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_SPAMERPENALTY_H
