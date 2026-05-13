// Server/DataReader/SHN/PupPriority.h
// Auto-generated: one-file-per-SHN split for PupPriority.shn
#ifndef SHINE_DATAREADER_SHN_PUPPRIORITY_H
#define SHINE_DATAREADER_SHN_PUPPRIORITY_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct PupPriorityRow {
    uint32           uiPupPriorityType;
    uint8            uiPriorityNum;
};

class PupPriorityShn {
public:
    static PupPriorityShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PupPriorityRow>& Rows() const { return m_kRows; }
private:
    std::vector<PupPriorityRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_PUPPRIORITY_H
