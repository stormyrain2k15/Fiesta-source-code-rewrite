// Server/DataReader/SHN/GTIGetRate.h
// Auto-generated: one-file-per-SHN split for GTIGetRate.shn
#ifndef FIESTA_DATAREADER_SHN_GTIGETRATE_H
#define FIESTA_DATAREADER_SHN_GTIGETRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GTIGetRateRow {
    uint8            uiID;
    uint8            uiWin1;
    uint8            uiLose1;
    uint8            uiWin2;
    uint8            uiLose2;
    uint8            uiWin3;
    uint8            uiLose3;
};

class GTIGetRateShn {
public:
    static GTIGetRateShn& Get();
    void Load();
    const GTIGetRateRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GTIGetRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<GTIGetRateRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GTIGETRATE_H
