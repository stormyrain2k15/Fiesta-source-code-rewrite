// Server/DataReader/SHN/TermExtendMatch.h
// Auto-generated: one-file-per-SHN split for TermExtendMatch.shn
#ifndef SHINE_DATAREADER_SHN_TERMEXTENDMATCH_H
#define SHINE_DATAREADER_SHN_TERMEXTENDMATCH_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct TermExtendMatchRow {
    std::string      kExtendItemIDX;
    std::string      kTermItemIDX;
};

class TermExtendMatchShn {
public:
    static TermExtendMatchShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<TermExtendMatchRow>& Rows() const { return m_kRows; }
private:
    std::vector<TermExtendMatchRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_TERMEXTENDMATCH_H
