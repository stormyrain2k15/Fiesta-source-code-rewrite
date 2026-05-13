// Server/DataReader/SHN/AdminLvSet.h
// Auto-generated: one-file-per-SHN split for AdminLvSet.shn
#ifndef SHINE_DATAREADER_SHN_ADMINLVSET_H
#define SHINE_DATAREADER_SHN_ADMINLVSET_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct AdminLvSetRow {
    std::string      kALS_Cmd;
    uint8            uiALS_Lv;
};

class AdminLvSetShn {
public:
    static AdminLvSetShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<AdminLvSetRow>& Rows() const { return m_kRows; }
private:
    std::vector<AdminLvSetRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ADMINLVSET_H
