// Server/DataReader/SHN/GTIGetRateGap.h
// Auto-generated: one-file-per-SHN split for GTIGetRateGap.shn
#ifndef SHINE_DATAREADER_SHN_GTIGETRATEGAP_H
#define SHINE_DATAREADER_SHN_GTIGETRATEGAP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GTIGetRateGapRow {
    uint8            uiID;
    uint16           uiGap;
};

class GTIGetRateGapShn {
public:
    static GTIGetRateGapShn& Get();
    void Load();
    const GTIGetRateGapRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GTIGetRateGapRow>& Rows() const { return m_kRows; }
private:
    std::vector<GTIGetRateGapRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GTIGETRATEGAP_H
