// Server/DataReader/SHN/MultiHitType.h
// Auto-generated: one-file-per-SHN split for MultiHitType.shn
#ifndef SHINE_DATAREADER_SHN_MULTIHITTYPE_H
#define SHINE_DATAREADER_SHN_MULTIHITTYPE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MultiHitTypeRow {
    uint16           uiID;
    uint16           uiHitTime;
    std::string      kAbIndex;
    uint8            uiAS_Step;
    uint8            uiAbStr;
    uint16           uiAbRate;
    uint16           uiDmgRate;
};

class MultiHitTypeShn {
public:
    static MultiHitTypeShn& Get();
    void Load();
    const MultiHitTypeRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MultiHitTypeRow>& Rows() const { return m_kRows; }
private:
    std::vector<MultiHitTypeRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MULTIHITTYPE_H
