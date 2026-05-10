// Server/DataReader/SHN/SpamerPenaltyRule.h
// Auto-generated: one-file-per-SHN split for SpamerPenaltyRule.shn
#ifndef FIESTA_DATAREADER_SHN_SPAMERPENALTYRULE_H
#define FIESTA_DATAREADER_SHN_SPAMERPENALTYRULE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct SpamerPenaltyRuleRow {
    uint16           uiSecondRule;
    uint8            uiRepeatRule;
};

class SpamerPenaltyRuleShn {
public:
    static SpamerPenaltyRuleShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<SpamerPenaltyRuleRow>& Rows() const { return m_kRows; }
private:
    std::vector<SpamerPenaltyRuleRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_SPAMERPENALTYRULE_H
