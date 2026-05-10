// Server/DataReader/SHN/PupMind.h
// Auto-generated: one-file-per-SHN split for PupMind.shn
#ifndef FIESTA_DATAREADER_SHN_PUPMIND_H
#define FIESTA_DATAREADER_SHN_PUPMIND_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct PupMindRow {
    uint32           uiPupMindType;
    uint8            uiMinMind;
    uint8            uiMaxMind;
};

class PupMindShn {
public:
    static PupMindShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PupMindRow>& Rows() const { return m_kRows; }
private:
    std::vector<PupMindRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_PUPMIND_H
