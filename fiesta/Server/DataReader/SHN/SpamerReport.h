// Server/DataReader/SHN/SpamerReport.h
// Auto-generated: one-file-per-SHN split for SpamerReport.shn
#ifndef FIESTA_DATAREADER_SHN_SPAMERREPORT_H
#define FIESTA_DATAREADER_SHN_SPAMERREPORT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct SpamerReportRow {
    uint32           uiSR_Term;
    uint16           uiSR_Number;
    std::string      kSR_Message;
};

class SpamerReportShn {
public:
    static SpamerReportShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<SpamerReportRow>& Rows() const { return m_kRows; }
private:
    std::vector<SpamerReportRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_SPAMERREPORT_H
