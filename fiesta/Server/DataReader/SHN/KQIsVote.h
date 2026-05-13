// Server/DataReader/SHN/KQIsVote.h
// Auto-generated: one-file-per-SHN split for KQIsVote.shn
#ifndef SHINE_DATAREADER_SHN_KQISVOTE_H
#define SHINE_DATAREADER_SHN_KQISVOTE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct KQIsVoteRow {
    int16            iID;
    uint8            uiIsVote;
};

class KQIsVoteShn {
public:
    static KQIsVoteShn& Get();
    void Load();
    const KQIsVoteRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<KQIsVoteRow>& Rows() const { return m_kRows; }
private:
    std::vector<KQIsVoteRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_KQISVOTE_H
