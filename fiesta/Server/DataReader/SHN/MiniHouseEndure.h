// Server/DataReader/SHN/MiniHouseEndure.h
// Auto-generated: one-file-per-SHN split for MiniHouseEndure.shn
#ifndef FIESTA_DATAREADER_SHN_MINIHOUSEENDURE_H
#define FIESTA_DATAREADER_SHN_MINIHOUSEENDURE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MiniHouseEndureRow {
    uint16           uiHandle;
    int16            iEndure;
    int16            iUnkCol2;
    int16            iUnkCol3;
    int16            iUnkCol4;
};

class MiniHouseEndureShn {
public:
    static MiniHouseEndureShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MiniHouseEndureRow>& Rows() const { return m_kRows; }
private:
    std::vector<MiniHouseEndureRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MINIHOUSEENDURE_H
