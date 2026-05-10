// Server/DataReader/SHN/TermExtendMatch.h
// Auto-generated: one-file-per-SHN split for TermExtendMatch.shn
#ifndef FIESTA_DATAREADER_SHN_TERMEXTENDMATCH_H
#define FIESTA_DATAREADER_SHN_TERMEXTENDMATCH_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

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

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_TERMEXTENDMATCH_H
