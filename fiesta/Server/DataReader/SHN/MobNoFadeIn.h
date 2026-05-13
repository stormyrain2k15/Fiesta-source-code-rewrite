// Server/DataReader/SHN/MobNoFadeIn.h
// Auto-generated: one-file-per-SHN split for MobNoFadeIn.shn
#ifndef SHINE_DATAREADER_SHN_MOBNOFADEIN_H
#define SHINE_DATAREADER_SHN_MOBNOFADEIN_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MobNoFadeInRow {
    std::string      kMobInx;
};

class MobNoFadeInShn {
public:
    static MobNoFadeInShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobNoFadeInRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobNoFadeInRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOBNOFADEIN_H
