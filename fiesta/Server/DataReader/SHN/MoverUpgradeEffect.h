// Server/DataReader/SHN/MoverUpgradeEffect.h
// Auto-generated: one-file-per-SHN split for MoverUpgradeEffect.shn
#ifndef SHINE_DATAREADER_SHN_MOVERUPGRADEEFFECT_H
#define SHINE_DATAREADER_SHN_MOVERUPGRADEEFFECT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MoverUpgradeEffectRow {
    uint16           uiRunSpeed;
    uint16           uiHPSPRecoveryTick;
    uint16           uiHPSPRecovery;
    uint16           uiCastingTime;
    uint16           uiCastingCoolTime;
    std::string      kEffectFileName;
    std::string      kAbStateIDX;
    uint8            uiStrength;
};

class MoverUpgradeEffectShn {
public:
    static MoverUpgradeEffectShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MoverUpgradeEffectRow>& Rows() const { return m_kRows; }
private:
    std::vector<MoverUpgradeEffectRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOVERUPGRADEEFFECT_H
