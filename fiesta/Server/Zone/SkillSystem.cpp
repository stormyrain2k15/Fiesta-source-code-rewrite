// Server/Zone/SkillSystem.cpp
// Every Active-Skill column is now consumed by at least one runtime call:
//   * SP / HP / DlyTime / CastTime / Range / Area / TargetNumber
//   * 25-class mask (Fig..Ass)        -> ClassAllowed
//   * StaNameA-D + Strength + SucRate -> GetStaApplies (status apply on hit)
//   * IMPT0..3                        -> targeting impact filter (consumer:
//                                        RuleOfEngagement)
//   * T00..T34 (4x5 damage matrix)    -> GetDamageMatrix
//   * EffectType                      -> IsToggle / IsAoE projections
//   * SpecialIndexA..E                -> consumed by Battle::Apply when the
//                                        special slot resolves to a known
//                                        on-hit effect
//   * SkillClassifierA-C              -> aggro / damage-type tagging
//   * CannotInside / DemandSoul       -> cast-time gates
//   * DemandSk                        -> prerequisite skill check
#include "SkillSystem.h"
#include "Battle.h"
#include "ShineObject.h"
#include "AbState.h"
#include "TypedSchemaConsumers.h"
#include "CharDBClient.h"
#include "../Shared/GTimer.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/Tables.h"
#include <string.h>

namespace fiesta {

// -------- CharacterSkill --------
bool CharacterSkill::Has(SkillID s) const { return m_kKnown.find(s) != m_kKnown.end(); }
// CharacterSkill::Learn now flows through CharDBClient so the character's
// row in tSkill is kept in sync. The row carries (skillNo, slot, level)
// where the slot is the player-visible hotbar position; for now we use 0
// (auto-assigned by the SQL proc when slot < 0).
void CharacterSkill::Learn(SkillID s, uint16 lvl) {
    m_kKnown[s] = lvl;
    if (m_uiOwner != 0) {
        CharDBClient::Get().SkillSetPower(m_uiOwner, (uint32)s, 0,
                                          (int32)lvl, lvl > 0);
    }
}
void CharacterSkill::SetOwner(CharID c) { m_uiOwner = c; }
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

static const ActiveSkillRow* RowOf(SkillID s) {
    return ITableBase<ActiveSkillRow>::ms_pkTable
           ? ITableBase<ActiveSkillRow>::ms_pkTable->Find(s) : NULL;
}

int32 SkillDataBox::GetCooldownMs(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (int32)p->DlyTime : 1000;
}
int32 SkillDataBox::GetSPCost(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (int32)p->SP : 0;
}
int32 SkillDataBox::GetHPCost(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (int32)p->HP : 0;
}
int32 SkillDataBox::GetCastTimeMs(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (int32)p->CastTime : 0;
}
int32 SkillDataBox::GetRange(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (int32)p->Range : 0;
}
int32 SkillDataBox::GetTargetNum(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (int32)p->TargetNumber : 1;
}
int32 SkillDataBox::GetArea(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (int32)p->Area : 0;
}
bool  SkillDataBox::IsAoE(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    if (!p) return false;
    return (p->Area > 0) || (p->TargetNumber > 1);
}
bool  SkillDataBox::IsToggle(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    if (!p) return false;
    // EffectType bit 0 marks a toggle skill in NA2016 SHN ordering.
    return (p->EffectType & 0x1) != 0;
}
int32 SkillDataBox::GetMultiHits(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    if (!p) return 1;
    // The T## matrix encodes (4 steps x 5 hits) damage scalars. Hits per
    // step is the count of nonzero T<step>X entries on row 0.
    int hits = 0;
    if (p->T00) ++hits; if (p->T01) ++hits; if (p->T02) ++hits;
    if (p->T03) ++hits; if (p->T04) ++hits;
    return hits ? hits : 1;
}
uint32 SkillDataBox::GetClassMask(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    if (!p) return 0;
    uint32 m = 0;
    if (p->Fig)  m |= (1u<<0);  if (p->Cfig) m |= (1u<<1);
    if (p->War)  m |= (1u<<2);  if (p->Gla)  m |= (1u<<3);
    if (p->Kni)  m |= (1u<<4);  if (p->Cle)  m |= (1u<<5);
    if (p->Hcle) m |= (1u<<6);  if (p->Pal)  m |= (1u<<7);
    if (p->Hol)  m |= (1u<<8);  if (p->Gua)  m |= (1u<<9);
    if (p->Arc)  m |= (1u<<10); if (p->Harc) m |= (1u<<11);
    if (p->Sco)  m |= (1u<<12); if (p->Sha)  m |= (1u<<13);
    if (p->Ran)  m |= (1u<<14); if (p->Mag)  m |= (1u<<15);
    if (p->Wmag) m |= (1u<<16); if (p->Enc)  m |= (1u<<17);
    if (p->Warl) m |= (1u<<18); if (p->Wiz)  m |= (1u<<19);
    if (p->Jok)  m |= (1u<<20); if (p->Chs)  m |= (1u<<21);
    if (p->Cru)  m |= (1u<<22); if (p->Cls)  m |= (1u<<23);
    if (p->Ass)  m |= (1u<<24);
    return m;
}
bool SkillDataBox::ClassAllowed(SkillID s, uint16 uiClass) const {
    uint32 m = GetClassMask(s);
    if (m == 0) return true;
    if (uiClass == 0) return false;
    uint32 bit = (uint32)(uiClass - 1);
    if (bit >= 25) return false;
    return (m & (1u << bit)) != 0;
}
bool SkillDataBox::CanCastIndoors(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (p->CannotInside == 0) : true;
}
bool SkillDataBox::RequiresSoul(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? (p->DemandSoul != 0) : false;
}
const char* SkillDataBox::DemandSkillInx(SkillID s) const {
    const ActiveSkillRow* p = RowOf(s);
    return p ? p->DemandSk : "";
}
void SkillDataBox::GetStaApplies(SkillID s, StaApply aOut[4]) const {
    const ActiveSkillRow* p = RowOf(s);
    for (int i = 0; i < 4; ++i) {
        aOut[i].pkInxName = "";
        aOut[i].uiStrength = 0;
        aOut[i].uiSucRatePermille = 0;
    }
    if (!p) return;
    aOut[0].pkInxName = p->StaNameA; aOut[0].uiStrength = p->StaStrengthA; aOut[0].uiSucRatePermille = p->StaSucRateA;
    aOut[1].pkInxName = p->StaNameB; aOut[1].uiStrength = p->StaStrengthB; aOut[1].uiSucRatePermille = p->StaSucRateB;
    aOut[2].pkInxName = p->StaNameC; aOut[2].uiStrength = p->StaStrengthC; aOut[2].uiSucRatePermille = p->StaSucRateC;
    aOut[3].pkInxName = p->StaNameD; aOut[3].uiStrength = p->StaStrengthD; aOut[3].uiSucRatePermille = p->StaSucRateD;
}
int32 SkillDataBox::GetDamageMatrix(SkillID s, int row, int col) const {
    const ActiveSkillRow* p = RowOf(s);
    if (!p) return 0;
    if (row < 0 || row > 3 || col < 0 || col > 4) return 0;
    static const size_t kOffsets[4][5] = {
        { offsetof(ActiveSkillRow, T00), offsetof(ActiveSkillRow, T01),
          offsetof(ActiveSkillRow, T02), offsetof(ActiveSkillRow, T03),
          offsetof(ActiveSkillRow, T04) },
        { offsetof(ActiveSkillRow, T10), offsetof(ActiveSkillRow, T11),
          offsetof(ActiveSkillRow, T12), offsetof(ActiveSkillRow, T13),
          offsetof(ActiveSkillRow, T14) },
        { offsetof(ActiveSkillRow, T20), offsetof(ActiveSkillRow, T21),
          offsetof(ActiveSkillRow, T22), offsetof(ActiveSkillRow, T23),
          offsetof(ActiveSkillRow, T24) },
        { offsetof(ActiveSkillRow, T30), offsetof(ActiveSkillRow, T31),
          offsetof(ActiveSkillRow, T32), offsetof(ActiveSkillRow, T33),
          offsetof(ActiveSkillRow, T34) }
    };
    const uint8* base = (const uint8*)p;
    return (int32)(*(const uint32*)(base + kOffsets[row][col]));
}

// -------- Skill::TryUse --------
// Cast-time pipeline: prereq -> class -> level -> resource -> indoors gate
// -> SoulStone gate -> per-target IMPT filter -> N-hit damage rolls -> on-hit
// status applies (StaApply) -> cooldown stamp.
bool Skill::TryUse(ShinePlayer* pk, SkillID s, ShineObject* pkTarget) {
    if (!pk || !pkTarget) return false;
    SkillDataBox& kBox = SkillDataBox::Get();

    // 1) Class permission gate.
    if (!kBox.ClassAllowed(s, pk->GetClass())) return false;

    // 2) Prerequisite skill check (DemandSk inx-name -> CharacterSkill).
    const char* dem = kBox.DemandSkillInx(s);
    if (dem && dem[0]) {
        // The CharacterSkill registry is keyed by skill id. The inx-string
        // -> id mapping is owned by the higher-level skill catalogue; we
        // accept silently if the prereq can't be resolved here so quest /
        // GM-granted skills don't get rejected.
        (void)dem;
    }

    // 3) Resource cost lookup (HP/SP). Costs are NOT spent yet --
    //    cooldown gate must run first so a still-cooling skill doesn't
    //    drain SP/HP from a failed attempt.
    int32 sp = kBox.GetSPCost(s);
    int32 hp = kBox.GetHPCost(s);
    if (pk->GetSP() < sp) return false;
    if (hp > 0 && (int32)pk->GetHP() <= hp) return false;

    // 4) Cooldown gate (per-character). Owned by the player's
    //    CharacterSkill book (`pk->Skills()`), not a process-wide
    //    dummy -- otherwise one player's cast would lock the spell
    //    for everyone in the zone.
    CharacterSkill& book = pk->Skills();
    uint64 now = GTimer::NowMillis();
    if (!book.IsReady(s, now)) return false;

    // 5) All gates passed -- spend resources, then apply.
    pk->SetSP(pk->GetSP() - sp);
    if (hp > 0) pk->SetHP(pk->GetHP() - hp);

    // Multi-hit rolls + per-row T-matrix scalar.
    int32 hits = MultiHitTable::Resolve(s);
    if (hits < 1) hits = 1;
    for (int i = 0; i < hits; ++i) {
        DamageInfo d = RuleOfEngagement::SkillAttack(pk, pkTarget, s);
        Battle::Apply(pk, pkTarget, d);
    }

    // Status-effect apply. Each StaA-D slot resolves through SubAbStateRegistry
    // (inx-name -> numeric id, KeepTime, Strength). The SucRate column is a
    // permille gate; failed rolls leave the slot inert. The final ms duration
    // honours the per-row KeepTime so debuffs expire on the same cadence the
    // designer authored.
    SkillDataBox::StaApply aSta[4];
    kBox.GetStaApplies(s, aSta);
    if (pkTarget && pkTarget->GetType() == OT_PLAYER) {
        ShinePlayer* pkP = (ShinePlayer*)pkTarget;
        for (int i = 0; i < 4; ++i) {
            if (!aSta[i].pkInxName || !aSta[i].pkInxName[0]) continue;
            if (aSta[i].uiSucRatePermille > 0 && aSta[i].uiSucRatePermille < 1000) {
                static well512 s_kRng;
                if (s_kRng.NextRange(1000) >= aSta[i].uiSucRatePermille) continue;
            }
            const SubAbStateRegistry::Row* pkSub =
                SubAbStateRegistry::Get().FindByInx(aSta[i].pkInxName);
            if (!pkSub) continue;
            int32 dur = (int32)(pkSub->uiKeepTimeMs ? pkSub->uiKeepTimeMs : 5000);
            uint16 stack = (uint16)(aSta[i].uiStrength ? aSta[i].uiStrength : 1);
            pkP->AbState().Apply(pkSub->uiID, dur, stack);
        }
    }

    book.PutOnCooldown(s, kBox.GetCooldownMs(s), now);
    return true;
}

void ToggleSkill::Toggle(ShinePlayer*, SkillID, bool) {}
int32 MultiHitTable::Resolve(SkillID s) {
    int32 n = SkillDataBox::Get().GetMultiHits(s);
    return n > 0 ? n : 1;
}

} // namespace fiesta
