// Server/Zone/SkillSystem.h
// 13 -- skills / cooldowns / multi-hit / AoE / toggle.
// EVIDENCE: PDB_CONFIRMED  symbol: Skill, CharacterSkill, SkillDataBox, DefSkillData,
//                                  MultiHitTable, MultiTypeHandle, ToggleSkill, ActiveSkillInfoServer
#ifndef FIESTA_ZONE_SKILLSYSTEM_H
#define FIESTA_ZONE_SKILLSYSTEM_H
#include "ShineObject.h"
#include <map>
#include <vector>

namespace fiesta {

struct SkillCooldown { SkillID uiSid; uint64 uiReadyAtMs; };

class CharacterSkill {
public:
    bool   Has(SkillID s) const;
    void   Learn(SkillID s, uint16 uiLevel);
    uint16 GetLevel(SkillID s) const;
    bool   IsReady(SkillID s, uint64 uiNowMs) const;
    void   PutOnCooldown(SkillID s, int32 iMs, uint64 uiNowMs);
private:
    std::map<SkillID, uint16>  m_kKnown;
    std::vector<SkillCooldown> m_kCooldowns;
};

class SkillDataBox {
public:
    static SkillDataBox& Get();
    int32  GetCooldownMs(SkillID s) const;
    int32  GetSPCost    (SkillID s) const;
    bool   IsToggle     (SkillID s) const;
    bool   IsAoE        (SkillID s) const;
    int32  GetMultiHits (SkillID s) const;
};

class Skill {
public:
    // server side cast-flow entry point
    static bool TryUse(ShinePlayer* pk, SkillID s, ShineObject* pkTarget);
};

class ToggleSkill {
public:
    static void Toggle(ShinePlayer* pk, SkillID s, bool bOn);
};

class MultiHitTable {
public:
    // Returns hits emitted; caller invokes RuleOfEngagement::SkillAttack per hit.
    static int32 Resolve(SkillID s);
};

} // namespace fiesta
#endif
