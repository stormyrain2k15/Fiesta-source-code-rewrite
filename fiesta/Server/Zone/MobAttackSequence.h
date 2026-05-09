// Server/Zone/MobAttackSequence.h
// 12/19 -- per-mob attack pattern files at MobAttackSequence/<MobName>.txt.
// These are plain-text sequence definitions used by the AI script to choose
// which skill / animation a mob fires next.
//
// Each file is owned by one MobInfo row (matched by name). The file is a list
// of action steps -- each step references a SkillID / animation key from the
// mob's Action/<MobName>.dat. The text format is line-based (one step per line)
// with the AI script consuming it through a simple cursor.
//
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied MobAttackSequence/.
#ifndef FIESTA_ZONE_MOBATTACKSEQUENCE_H
#define FIESTA_ZONE_MOBATTACKSEQUENCE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobAttackStep {
    std::string kKey;       // skill / action key, free-form (resolved against Action .dat)
    std::vector<std::string> kArgs;
};

class MobAttackSequence {
public:
    bool   Load(const std::string& rPath);
    size_t Steps() const { return m_kSteps.size(); }
    const MobAttackStep& At(size_t i) const { return m_kSteps[i]; }
private:
    std::vector<MobAttackStep> m_kSteps;
};

class MobAttackSequenceBox {
public:
    static MobAttackSequenceBox& Get();
    const MobAttackSequence* Load(const std::string& rRoot, const std::string& rMobName);
    const MobAttackSequence* Find(const std::string& rMobName) const;
    void   Clear();
private:
    std::map<std::string, MobAttackSequence*> m_kAll;
};

} // namespace fiesta
#endif
