// Server/DataReader/SHN/PartyBonusByMember.h
// Auto-generated: one-file-per-SHN split for PartyBonusByMember.shn
#ifndef FIESTA_DATAREADER_SHN_PARTYBONUSBYMEMBER_H
#define FIESTA_DATAREADER_SHN_PARTYBONUSBYMEMBER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct PartyBonusByMemberRow {
    uint8            uiPB_PartyMember;
    uint16           uiPB_BonusRatio;
};

class PartyBonusByMemberShn {
public:
    static PartyBonusByMemberShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PartyBonusByMemberRow>& Rows() const { return m_kRows; }
private:
    std::vector<PartyBonusByMemberRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_PARTYBONUSBYMEMBER_H
