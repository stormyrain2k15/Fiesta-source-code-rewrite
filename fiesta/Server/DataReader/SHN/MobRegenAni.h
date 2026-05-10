// Server/DataReader/SHN/MobRegenAni.h
// Auto-generated: one-file-per-SHN split for MobRegenAni.shn
#ifndef FIESTA_DATAREADER_SHN_MOBREGENANI_H
#define FIESTA_DATAREADER_SHN_MOBREGENANI_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobRegenAniRow {
    std::string      kMobIDX;
    uint16           uiRegenTime;
    std::string      kGroupAbStateIDX;
    uint8            uiIsAggro;
};

class MobRegenAniShn {
public:
    static MobRegenAniShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobRegenAniRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobRegenAniRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOBREGENANI_H
