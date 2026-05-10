// Server/DataReader/SHN/PupCase.h
// Auto-generated: one-file-per-SHN split for PupCase.shn
#ifndef FIESTA_DATAREADER_SHN_PUPCASE_H
#define FIESTA_DATAREADER_SHN_PUPCASE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct PupCaseRow {
    uint32           uiPupCaseType;
    uint32           uiPupMindType;
    uint8            uiStressNum;
};

class PupCaseShn {
public:
    static PupCaseShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PupCaseRow>& Rows() const { return m_kRows; }
private:
    std::vector<PupCaseRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_PUPCASE_H
