// Server/DataReader/SHN/GBSMNPC.h
// Auto-generated: one-file-per-SHN split for GBSMNPC.shn
#ifndef SHINE_DATAREADER_SHN_GBSMNPC_H
#define SHINE_DATAREADER_SHN_GBSMNPC_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBSMNPCRow {
    std::string      kGBSM_MobInx;
    uint32           uiGBSMBet;
};

class GBSMNPCShn {
public:
    static GBSMNPCShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBSMNPCRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBSMNPCRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBSMNPC_H
