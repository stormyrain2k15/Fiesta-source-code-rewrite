// Server/DataReader/SHN/MobAutoAction.h
// Auto-generated: one-file-per-SHN split for MobAutoAction.shn
#ifndef FIESTA_DATAREADER_SHN_MOBAUTOACTION_H
#define FIESTA_DATAREADER_SHN_MOBAUTOACTION_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobAutoActionRow {
    std::string      kMobInx;
    uint32           uiAttack;
    uint32           uiTarget;
    uint32           uiActionType;
    std::string      kStateInx;
    uint8            uiStrength;
    uint16           uiEffectRate;
    uint16           uiRange;
};

class MobAutoActionShn {
public:
    static MobAutoActionShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobAutoActionRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobAutoActionRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOBAUTOACTION_H
