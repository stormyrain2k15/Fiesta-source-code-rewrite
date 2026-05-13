// Server/DataReader/SHN/PupCaseDesc.h
// Auto-generated: one-file-per-SHN split for PupCaseDesc.shn
#ifndef SHINE_DATAREADER_SHN_PUPCASEDESC_H
#define SHINE_DATAREADER_SHN_PUPCASEDESC_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct PupCaseDescRow {
    uint32           uiPupPriorityType;
    uint32           uiPupCaseType;
    std::string      kPupIDX;
    uint32           uiPupAIType;
    std::string      kSM_Inx;
    uint32           uiActionEffectID;
    std::string      kSoundFile;
};

class PupCaseDescShn {
public:
    static PupCaseDescShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PupCaseDescRow>& Rows() const { return m_kRows; }
private:
    std::vector<PupCaseDescRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_PUPCASEDESC_H
