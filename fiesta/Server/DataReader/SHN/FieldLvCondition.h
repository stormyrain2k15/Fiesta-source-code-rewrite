// Server/DataReader/SHN/FieldLvCondition.h
// Auto-generated: one-file-per-SHN split for FieldLvCondition.shn
#ifndef SHINE_DATAREADER_SHN_FIELDLVCONDITION_H
#define SHINE_DATAREADER_SHN_FIELDLVCONDITION_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct FieldLvConditionRow {
    std::string      kMapIndex;
    uint8            uiModeIDLv;
    uint16           uiLvFrom;
    uint16           uiLvTo;
};

class FieldLvConditionShn {
public:
    static FieldLvConditionShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<FieldLvConditionRow>& Rows() const { return m_kRows; }
private:
    std::vector<FieldLvConditionRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_FIELDLVCONDITION_H
