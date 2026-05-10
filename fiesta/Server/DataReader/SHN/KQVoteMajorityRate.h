// Server/DataReader/SHN/KQVoteMajorityRate.h
// Auto-generated: one-file-per-SHN split for KQVoteMajorityRate.shn
#ifndef FIESTA_DATAREADER_SHN_KQVOTEMAJORITYRATE_H
#define FIESTA_DATAREADER_SHN_KQVOTEMAJORITYRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct KQVoteMajorityRateRow {
    uint8            uiVoteAgreeRate;
};

class KQVoteMajorityRateShn {
public:
    static KQVoteMajorityRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<KQVoteMajorityRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<KQVoteMajorityRateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_KQVOTEMAJORITYRATE_H
