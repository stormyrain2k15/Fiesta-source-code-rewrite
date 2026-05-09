// Server/Zone/SkillSystem.cpp
#include "SkillSystem.h"
#include "Battle.h"
#include "../Shared/GTimer.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/Tables.h"  // back-compat aliases

namespace fiesta {

// -------- CharacterSkill --------
bool CharacterSkill::Has(SkillID s) const { return m_kKnown.find(s) != m_kKnown.end(); }
void CharacterSkill::Learn(SkillID s, uint16 lvl) { m_kKnown[s] = lvl; }
uint16 CharacterSkill::GetLevel(SkillID s) const {
    std::map<SkillID, uint16>::const_iterator it = m_kKnown.find(s);
    return (it == m_kKnown.end()) ? 0 : it->second;
}
bool CharacterSkill::IsReady(SkillID s, uint64 uiNow) const {
    for (size_t i = 0; i < m_kCooldowns.size(); ++i)
        if (m_kCooldowns[i].uiSid == s) return uiNow >= m_kCooldowns[i].uiReadyAtMs;
    return true;
}
void CharacterSkill::PutOnCooldown(SkillID s, int32 ms, uint64 uiNow) {
    for (size_t i = 0; i < m_kCooldowns.size(); ++i)
        if (m_kCooldowns[i].uiSid == s) { m_kCooldowns[i].uiReadyAtMs = uiNow + ms; return; }
    SkillCooldown c; c.uiSid = s; c.uiReadyAtMs = uiNow + ms; m_kCooldowns.push_back(c);
}

// -------- SkillDataBox --------
SkillDataBox& SkillDataBox::Get() { static SkillDataBox s; return s; }

int32 SkillDataBox::GetCooldownMs(SkillID s) const {
    const ActiveSkillRow* p = ITableBase<ActiveSkillRow>::ms_pkTable
                              ? ITableBase<ActiveSkillRow>::ms_pkTable->Find(s) : NULL;
    // Documentation has a CoolTime/RecastTime field; until we know the exact
    // member name in Schemas.h, fall back to a provisional default.
    (void)p;
    return 1000; // EV_VERIFY default
}
int32 SkillDataBox::GetSPCost(SkillID) const { return 5; }      // EV_VERIFY
bool  SkillDataBox::IsToggle (SkillID) const { return false; }  // EV_VERIFY
bool  SkillDataBox::IsAoE    (SkillID) const { return false; }  // EV_VERIFY

// -------- Skill --------
bool Skill::TryUse(ShinePlayer* pk, SkillID s, ShineObject* pkTarget) {
    if (!pk || !pkTarget) return false;
    static CharacterSkill s_kDummy; // pass-1: per-player CharacterSkill not yet attached
    uint64 now = GTimer::NowMillis();
    if (!s_kDummy.IsReady(s, now)) return false;
    int32 cost = SkillDataBox::Get().GetSPCost(s);
    if (pk->GetSP() < cost) return false;
    pk->SetSP(pk->GetSP() - cost);
    int32 hits = MultiHitTable::Resolve(s); if (hits < 1) hits = 1;
    for (int i = 0; i < hits; ++i) {
        DamageInfo d = RuleOfEngagement::SkillAttack(pk, pkTarget, s);
        Battle::Apply(pk, pkTarget, d);
    }
    s_kDummy.PutOnCooldown(s, SkillDataBox::Get().GetCooldownMs(s), now);
    return true;
}

void ToggleSkill::Toggle(ShinePlayer*, SkillID, bool) {}
int32 MultiHitTable::Resolve(SkillID) { return 1; } // EV_VERIFY

} // namespace fiesta
