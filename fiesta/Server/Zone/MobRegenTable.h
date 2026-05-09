// Server/Zone/MobRegenTable.h
// 12 -- typed loader for MobRegen/<Map>.txt (MobRegenGroup + MobRegen tables).
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied MobRegen/.
#ifndef FIESTA_ZONE_MOBREGENTABLE_H
#define FIESTA_ZONE_MOBREGENTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobRegenGroupRow {
    std::string kGroupIndex;     // INDEX
    std::string kIsFamily;       // STRING[1] -- "Y" or "N"
    uint32      uiCenterX, uiCenterY;
    uint32      uiWidth,   uiHeight;
    uint32      uiRangeDegree;
};

struct MobRegenRow {
    std::string kRegenIndex;     // group-id reference
    std::string kMobIndex;       // STRING[33] -- mob name
    uint8       uiMobNum;
    uint8       uiKillNum;
    uint32      uiRegStandard;
    uint32      uiRegMin, uiRegMax;
    int32       aDelta[6];       // RegDelta0..RegDelta5
    int32       aSec  [6];       // RegSec0..RegSec5
};

class MobRegenTable {
public:
    bool Load(const std::string& rPath);
    const std::vector<MobRegenGroupRow>& Groups() const { return m_kGroups; }
    const std::vector<MobRegenRow>&      Regens() const { return m_kRegens; }
private:
    std::vector<MobRegenGroupRow> m_kGroups;
    std::vector<MobRegenRow>      m_kRegens;
};

class MobRegenBox {
public:
    static MobRegenBox& Get();
    const MobRegenTable* Load(const std::string& rRoot, const std::string& rMapId);
    const MobRegenTable* Find(const std::string& rMapId) const;
    void Clear();
private:
    std::map<std::string, MobRegenTable*> m_kAll;
};

} // namespace fiesta
#endif
