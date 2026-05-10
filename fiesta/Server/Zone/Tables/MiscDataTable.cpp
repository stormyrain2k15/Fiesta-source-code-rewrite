// Server/Zone/Tables/MiscDataTable.cpp
// FEATURE: world-creation -- World/MiscDataTable.txt binder.
#include "MiscDataTable.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MiscDataTable& MiscDataTable::Get() { static MiscDataTable s; return s; }

bool MiscDataTable::Load(const std::string& rRoot) {
    m_kRows.clear(); m_kBySkill.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\MiscDataTable.txt")) return false;
    const TsTable* t = f.Find("SkillBreedMob"); if (!t) return false;
    // FEATURE: world-creation -- column read: Serial, Skill, MobIndex,
    // SummonNum, AI, RegenLoc, RegenDistance, MobRoam, LifeTime,
    // AbState, TriggerObject, TriggerRange, Delay, ExplNo, Explosion,
    // ExplosionWhere, Debuff, MultiTarget
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        SkillBreedMobRow x;
        x.uiSerial        = (uint8) t->GetInt(r, "Serial");
        x.kSkill          = t->GetStr(r, "Skill");
        if (x.kSkill.empty() || x.kSkill == "-") continue;
        x.kMobIndex       = t->GetStr(r, "MobIndex");
        x.uiSummonNum     = (uint8) t->GetInt(r, "SummonNum");
        x.kAI             = t->GetStr(r, "AI");
        x.kRegenLoc       = t->GetStr(r, "RegenLoc");
        x.uiRegenDistance = (uint16)t->GetInt(r, "RegenDistance");
        x.kMobRoam        = t->GetStr(r, "MobRoam");
        x.uiLifeTimeMs    = (uint32)t->GetInt(r, "LifeTime");
        x.kAbState        = t->GetStr(r, "AbState");
        x.kTriggerObject  = t->GetStr(r, "TriggerObject");
        x.uiTriggerRange  = (uint32)t->GetInt(r, "TriggerRange");
        x.uiDelayMs       = (uint16)t->GetInt(r, "Delay");
        x.uiExplNo        = (uint16)t->GetInt(r, "ExplNo");
        x.kExplosion      = t->GetStr(r, "Explosion");
        x.kExplosionWhere = t->GetStr(r, "ExplosionWhere");
        x.kDebuff         = t->GetStr(r, "Debuff");
        x.kMultiTarget    = t->GetStr(r, "MultiTarget");
        m_kBySkill[x.kSkill].push_back(m_kRows.size());
        m_kRows.push_back(x);
    }
    SHINELOG_INFO("MiscDataTable.txt: %u skill-breed-mob rows",
                  (uint32)m_kRows.size());
    return true;
}

void MiscDataTable::RowsForSkill(const std::string& rSkill,
                                 std::vector<const SkillBreedMobRow*>& rOut) const {
    rOut.clear();
    std::map<std::string, std::vector<size_t> >::const_iterator it =
        m_kBySkill.find(rSkill);
    if (it == m_kBySkill.end()) return;
    for (size_t i = 0; i < it->second.size(); ++i)
        rOut.push_back(&m_kRows[it->second[i]]);
}

} // namespace fiesta
