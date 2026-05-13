// Server/DataReader/SHN/AbStateSaveTypeInfo.h
// Auto-generated: one-file-per-SHN split for AbStateSaveTypeInfo.shn
#ifndef SHINE_DATAREADER_SHN_ABSTATESAVETYPEINFO_H
#define SHINE_DATAREADER_SHN_ABSTATESAVETYPEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct AbStateSaveTypeInfoRow {
    uint32           uiAbStateSaveType;
    uint8            uiIsSaveLink;
    uint8            uiIsSaveDie;
    uint8            uiIsSaveLogoff;
};

class AbStateSaveTypeInfoShn {
public:
    static AbStateSaveTypeInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<AbStateSaveTypeInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<AbStateSaveTypeInfoRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ABSTATESAVETYPEINFO_H
