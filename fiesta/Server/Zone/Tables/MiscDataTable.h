// Server/Zone/Tables/MiscDataTable.h
// FEATURE: world-creation -- World/MiscDataTable.txt declarations.
// Holds SkillBreedMob: when a skill (Entrap, SlowShot, Summon...) fires,
// it spawns a sub-mob with these settings (AI, regen-loc, lifetime,
// trigger object, explosion fx, multi-target flag).
#ifndef FIESTA_ZONE_TABLES_MISCDATATABLE_H
#define FIESTA_ZONE_TABLES_MISCDATATABLE_H
#include "../../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct SkillBreedMobRow {
    uint8       uiSerial;
    std::string kSkill;
    std::string kMobIndex;
    uint8       uiSummonNum;
    std::string kAI;            // Root, Chase, Rampage, Roam, ...
    std::string kRegenLoc;      // Caster, Target, ...
    uint16      uiRegenDistance;
    std::string kMobRoam;
    uint32      uiLifeTimeMs;
    std::string kAbState;
    std::string kTriggerObject; // Enemy, Target, Always, ...
    uint32      uiTriggerRange;
    uint16      uiDelayMs;
    uint16      uiExplNo;
    std::string kExplosion;
    std::string kExplosionWhere; // MyObj, TargetObj
    std::string kDebuff;
    std::string kMultiTarget;
};

class MiscDataTable {
public:
    static MiscDataTable& Get();
    bool Load(const std::string& rRoot);
    void RowsForSkill(const std::string& rSkill,
                      std::vector<const SkillBreedMobRow*>& rOut) const;
    size_t Count() const { return m_kRows.size(); }
private:
    MiscDataTable() {}
    std::vector<SkillBreedMobRow> m_kRows;
    std::map<std::string, std::vector<size_t> > m_kBySkill;
};

} // namespace fiesta
#endif
