// Server/DataReader/SHN/MoverMain.h
// Auto-generated: one-file-per-SHN split for MoverMain.shn
#ifndef SHINE_DATAREADER_SHN_MOVERMAIN_H
#define SHINE_DATAREADER_SHN_MOVERMAIN_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MoverMainRow {
    uint32           uiMoverID;
    std::string      kMoverIDX;
    uint32           uiCastingTime;
    uint32           uiCoolTime;
    uint16           uiRunSpeed;
    uint16           uiWalkSpeed;
    uint16           uiDurationHour;
    uint8            uiMaxCharSlot;
};

class MoverMainShn {
public:
    static MoverMainShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MoverMainRow>& Rows() const { return m_kRows; }
private:
    std::vector<MoverMainRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOVERMAIN_H
