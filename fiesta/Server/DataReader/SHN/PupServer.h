// Server/DataReader/SHN/PupServer.h
// Auto-generated: one-file-per-SHN split for PupServer.shn
#ifndef FIESTA_DATAREADER_SHN_PUPSERVER_H
#define FIESTA_DATAREADER_SHN_PUPSERVER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct PupServerRow {
    std::string      kPupIDX;
    uint8            uiMinMind;
    uint8            uiMaxMind;
    uint8            uiMinStress;
    uint8            uiMaxStress;
};

class PupServerShn {
public:
    static PupServerShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PupServerRow>& Rows() const { return m_kRows; }
private:
    std::vector<PupServerRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_PUPSERVER_H
