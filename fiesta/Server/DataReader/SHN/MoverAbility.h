// Server/DataReader/SHN/MoverAbility.h
// Auto-generated: one-file-per-SHN split for MoverAbility.shn
#ifndef SHINE_DATAREADER_SHN_MOVERABILITY_H
#define SHINE_DATAREADER_SHN_MOVERABILITY_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MoverAbilityRow {
    std::string      kMoverIDX;
    uint8            uiMoverLv;
    uint32           uiHP;
    uint32           uiWCMin;
    uint32           uiWCMax;
    uint32           uiMAMin;
    uint32           uiMAMax;
    uint16           uiAC;
    uint16           uiMR;
    uint16           uiTH;
    uint16           uiTB;
    std::string      kResIndex;
    uint32           uiAbsoluteSize;
};

class MoverAbilityShn {
public:
    static MoverAbilityShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MoverAbilityRow>& Rows() const { return m_kRows; }
private:
    std::vector<MoverAbilityRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOVERABILITY_H
