// Server/DataReader/SHN/ReactionType.h
// Auto-generated: one-file-per-SHN split for ReactionType.shn
#ifndef FIESTA_DATAREADER_SHN_REACTIONTYPE_H
#define FIESTA_DATAREADER_SHN_REACTIONTYPE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ReactionTypeRow {
    uint32           uiRAType;
    std::string      kMobInx;
    uint32           uiEcode;
};

class ReactionTypeShn {
public:
    static ReactionTypeShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ReactionTypeRow>& Rows() const { return m_kRows; }
private:
    std::vector<ReactionTypeRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_REACTIONTYPE_H
