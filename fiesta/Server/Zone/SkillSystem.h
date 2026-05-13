// Server/Zone/SkillSystem.h
// skills / cooldowns / multi-hit / AoE / toggle.
//                                  MultiHitTable, MultiTypeHandle, ToggleSkill, ActiveSkillInfoServer
#ifndef SHINE_ZONE_SKILLSYSTEM_H
#define SHINE_ZONE_SKILLSYSTEM_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>

namespace shine {

class ShineObject;
class ShinePlayer;

struct SkillCooldown { SkillID uiSid; uint64 uiReadyAtMs; };

class CharacterSkill {
public:
    CharacterSkill() : m_uiOwner(0) {}
    void   SetOwner(CharID c);
    bool   Has(SkillID s) const;
    void   Learn(SkillID s, uint16 uiLevel);
    uint16 GetLevel(SkillID s) const;
    bool   IsReady(SkillID s, uint64 uiNowMs) const;
    void   PutOnCooldown(SkillID s, int32 iMs, uint64 uiNowMs);
private:
    std::map<SkillID, uint16>  m_kKnown;
    std::vector<SkillCooldown> m_kCooldowns;
    CharID                     m_uiOwner;
};

class SkillDataBox {
public:
    static SkillDataBox& Get();
    int32  GetCooldownMs(SkillID s) const;
    int32  GetSPCost    (SkillID s) const;
    int32  GetHPCost    (SkillID s) const;
    int32  GetCastTimeMs(SkillID s) const;
    int32  GetRange     (SkillID s) const;
    int32  GetTargetNum (SkillID s) const;
    int32  GetArea      (SkillID s) const;
    bool   IsToggle     (SkillID s) const;
    bool   IsAoE        (SkillID s) const;
    int32  GetMultiHits (SkillID s) const;
    // ActiveSkill class-permission mask (same packing as ItemInfo).
    uint32 GetClassMask (SkillID s) const;
    bool   ClassAllowed (SkillID s, uint16 uiClass) const;
    // CannotInside / DemandSoul gates.
    bool   CanCastIndoors  (SkillID s) const;
    bool   RequiresSoul    (SkillID s) const;
    // Required prerequisite skill (DemandSk). Returns the skill inx string;
    // empty means no prereq.
    const char* DemandSkillInx(SkillID s) const;
    // Status-effect application on hit. Returns the four (StaName, Strength,
    // SucRate) tuples. Empty StaName == disabled slot.
    struct StaApply {
        const char* pkInxName;
        uint32      uiStrength;
        uint32      uiSucRatePermille;
    };
    void   GetStaApplies(SkillID s, StaApply aOut[4]) const;
    // Damage scalar matrix T00..T34 (4 rows x 5 cols). The skill cast
    // pipeline indexes by (skill-step, hit-index). All 20 are surfaced.
    int32  GetDamageMatrix(SkillID s, int row, int col) const;
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

} // namespace shine
#endif
