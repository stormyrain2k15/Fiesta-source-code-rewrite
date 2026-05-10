// Server/DataReader/SHN/MysteryVaultServer.h
// Auto-generated: one-file-per-SHN split for MysteryVaultServer.shn
#ifndef FIESTA_DATAREADER_SHN_MYSTERYVAULTSERVER_H
#define FIESTA_DATAREADER_SHN_MYSTERYVAULTSERVER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MysteryVaultServerRow {
    std::string      kMV_GroupItemInx;
    uint32           uiChrClass;
    std::string      kItemInx;
    uint8            uiLot;
    uint16           uiRate;
    uint8            uiGroup;
};

class MysteryVaultServerShn {
public:
    static MysteryVaultServerShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MysteryVaultServerRow>& Rows() const { return m_kRows; }
private:
    std::vector<MysteryVaultServerRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MYSTERYVAULTSERVER_H
