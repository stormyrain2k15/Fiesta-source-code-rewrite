// Server/DataReader/SHN/NpcDialogData.h
// Auto-generated: one-file-per-SHN split for NpcDialogData.shn
#ifndef FIESTA_DATAREADER_SHN_NPCDIALOGDATA_H
#define FIESTA_DATAREADER_SHN_NPCDIALOGDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct NpcDialogDataRow {
    uint32           uiID;
    std::string      kInxName;
};

class NpcDialogDataShn {
public:
    static NpcDialogDataShn& Get();
    void Load();
    const NpcDialogDataRow* FindById(uint32 uiID) const;
    const NpcDialogDataRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<NpcDialogDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<NpcDialogDataRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_NPCDIALOGDATA_H
