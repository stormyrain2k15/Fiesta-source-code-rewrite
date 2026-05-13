// Server/DataReader/SHN/PartyBonusLimit.h
// Auto-generated: one-file-per-SHN split for PartyBonusLimit.shn
#ifndef SHINE_DATAREADER_SHN_PARTYBONUSLIMIT_H
#define SHINE_DATAREADER_SHN_PARTYBONUSLIMIT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct PartyBonusLimitRow {
    uint8            uiPSE_ChrLv;
    uint32           uiPSE_ExpLimit;
};

class PartyBonusLimitShn {
public:
    static PartyBonusLimitShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PartyBonusLimitRow>& Rows() const { return m_kRows; }
private:
    std::vector<PartyBonusLimitRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_PARTYBONUSLIMIT_H
