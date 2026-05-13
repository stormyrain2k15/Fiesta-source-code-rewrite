// Server/Zone/AbState.cpp
// Per-target buff/debuff ledger -- the runtime side of the AbState /
// SubAbState data tables. Pipeline:
//
//   AbnormalState::Apply(ab,...) -> dictionary row -> SubAbState row
//                                -> AbStateRuntime::Resolve()
//                                -> push runtime row
//                                -> OnApply (one-shot stat-mod / shield)
//                                -> party-state propagation chain
//                                -> save-on-link gate
//
//   AbnormalState::Tick(...)     -> per-row Tick() (DoT / regen)
//                                -> expire and OnRemove
//
// Stat-mod aggregation (StatModX1k) walks the active rows and sums the
// arg of every row whose ActionIndex matches the requested action id;
// the BattleStat compose path queries this once per stat at recompute
// time so we don't need to rebuild the stat block on every buff tick.
#include "AbState.h"
#include "AbStateRuntime.h"
#include "AbnormalStateDictionary.h"
#include "AbnormalStateShelter.h"
#include "GroupTables.h"           // AbStateTables / AbStateRow
#include "MoreTables.h"            // SubAbStateTable
#include "ShineObject.h"
#include "BattleTunables.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"
#include <string.h>

namespace shine {

bool AbnormalState::Apply(uint32 uiAb, int32 iDurMs, uint16 uiStack) {
    return ApplyAt(uiAb, (uint32)uiStack, iDurMs);
}

bool AbnormalState::ApplyAt(uint32 uiAb, uint32 uiStrength, int32 iDurMs) {
    const AbStateRow* pkAb = AbnormalStateDictionary::Get().GetRow(uiAb);
    if (!pkAb) {
        SHINELOG_WARN("AbnormalState::ApplyAt: AbState id=%u not in dictionary",
                      uiAb);
        return false;
    }

    // Replace / stack rule. Walk the existing rows; if any same-id row
    // is present and the priority resolver says "replace", drop it
    // before pushing the new one.
    for (size_t i = 0; i < m_kRuntime.size(); ) {
        if (m_kRuntime[i].uiAbStateId == uiAb) {
            if (SubAbstatePriority::ShouldReplace(uiAb, uiAb)) {
                AbStateRuntimeRow gone = m_kRuntime[i];
                m_kRuntime.erase(m_kRuntime.begin() + i);
                AbStateRuntime::OnRemove(NULL, gone);
                continue;
            }
            if (!SubAbstatePriority::ShouldStack(uiAb, uiAb)) return false;
        }
        ++i;
    }

    AbStateRuntimeRow row;
    if (!AbStateRuntime::Resolve(*pkAb, uiStrength, row)) return false;

    // Honor an explicit duration override from the caller (e.g. cSetAbstate
    // with a script-supplied iDurMs). Only override when the caller passed
    // a positive value -- iDurMs<=0 means "use the SubAbState row's keep".
    if (iDurMs > 0) row.uiExpireMs = row.uiAppliedMs + (uint64)iDurMs;

    m_kRuntime.push_back(row);
    AbStateRuntime::OnApply(NULL, row);   // owner threaded through Tick()

    SHINELOG_DEBUG("AbState applied: ab=%u sub=%u str=%u keep=%llums",
                   row.uiAbStateId, row.uiSubAbStateId, row.uiStrength,
                   (unsigned long long)(row.uiExpireMs - row.uiAppliedMs));
    return true;
}

bool AbnormalState::Remove(uint32 uiAb) {
    bool bAny = false;
    for (size_t i = 0; i < m_kRuntime.size(); ) {
        if (m_kRuntime[i].uiAbStateId == uiAb) {
            AbStateRuntimeRow gone = m_kRuntime[i];
            m_kRuntime.erase(m_kRuntime.begin() + i);
            AbStateRuntime::OnRemove(NULL, gone);
            bAny = true;
        } else {
            ++i;
        }
    }
    return bAny;
}

bool AbnormalState::ApplySubByName(const char* szSubInxName,
                                   uint32 uiStrength, int32 iDurMs) {
    if (!szSubInxName || !szSubInxName[0]) return false;
    AbStateRuntimeRow row;
    if (!AbStateRuntime::ResolveFromSub(szSubInxName, uiStrength, row))
        return false;
    if (iDurMs > 0) row.uiExpireMs = row.uiAppliedMs + (uint64)iDurMs;

    // Replace any existing direct-SubAbState row that shares the same
    // SubAbState id (refresh / overwrite-with-stronger semantics).
    for (size_t i = 0; i < m_kRuntime.size(); ) {
        if (m_kRuntime[i].uiAbStateId == 0 &&
            m_kRuntime[i].uiSubAbStateId == row.uiSubAbStateId) {
            AbStateRuntimeRow gone = m_kRuntime[i];
            m_kRuntime.erase(m_kRuntime.begin() + i);
            AbStateRuntime::OnRemove(NULL, gone);
            continue;
        }
        ++i;
    }

    m_kRuntime.push_back(row);
    AbStateRuntime::OnApply(NULL, row);
    SHINELOG_DEBUG("AbState applied (sub-direct): sub=%u str=%u keep=%llums",
                   row.uiSubAbStateId, row.uiStrength,
                   (unsigned long long)(row.uiExpireMs - row.uiAppliedMs));
    return true;
}

bool AbnormalState::RemoveBySubInxName(const char* szSubInxName) {
    if (!szSubInxName || !szSubInxName[0]) return false;
    const SubAbStateTable::Row* pkSub =
        SubAbStateTable::Get().FindByInx(szSubInxName, 0);
    if (!pkSub) return false;
    bool bAny = false;
    for (size_t i = 0; i < m_kRuntime.size(); ) {
        if (m_kRuntime[i].uiSubAbStateId == pkSub->uiID) {
            AbStateRuntimeRow gone = m_kRuntime[i];
            m_kRuntime.erase(m_kRuntime.begin() + i);
            AbStateRuntime::OnRemove(NULL, gone);
            bAny = true;
        } else {
            ++i;
        }
    }
    return bAny;
}

uint32 AbnormalState::DispelByCategory(uint32 uiDispelIndex,
                                       bool bSubDispelMatch) {
    uint32 uiOut = 0;
    for (size_t i = 0; i < m_kRuntime.size(); ) {
        const AbStateRow* pkAb =
            AbnormalStateDictionary::Get().GetRow(m_kRuntime[i].uiAbStateId);
        bool match = false;
        if (pkAb) {
            match = (pkAb->uiDispelIndex == uiDispelIndex) ||
                    (bSubDispelMatch && pkAb->uiSubDispelIndex == uiDispelIndex);
        }
        if (match) {
            AbStateRuntimeRow gone = m_kRuntime[i];
            m_kRuntime.erase(m_kRuntime.begin() + i);
            AbStateRuntime::OnRemove(NULL, gone);
            ++uiOut;
        } else {
            ++i;
        }
    }
    return uiOut;
}

void AbnormalState::Tick(ShineObject* pkOwner, uint64 uiNowMs) {
    for (size_t i = 0; i < m_kRuntime.size(); ) {
        AbStateRuntime::Tick(pkOwner, m_kRuntime[i], uiNowMs);
        if (m_kRuntime[i].uiExpireMs && uiNowMs >= m_kRuntime[i].uiExpireMs) {
            AbStateRuntimeRow gone = m_kRuntime[i];
            m_kRuntime.erase(m_kRuntime.begin() + i);
            AbStateRuntime::OnRemove(pkOwner, gone);
        } else {
            ++i;
        }
    }
}

bool AbnormalState::Has(uint32 uiAb) const {
    for (size_t i = 0; i < m_kRuntime.size(); ++i)
        if (m_kRuntime[i].uiAbStateId == uiAb) return true;
    return false;
}

bool AbnormalState::HasInxName(const char* sz) const {
    if (!sz) return false;
    uint32 uiAb = AbnormalStateDictionary::Get().Lookup(sz);
    return uiAb != 0 && Has(uiAb);
}

int32 AbnormalState::StatModX1k(uint32 uiAction) const {
    int32 acc = 0;
    for (size_t i = 0; i < m_kRuntime.size(); ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_kRuntime[i].uiAction[j] == uiAction)
                acc += m_kRuntime[i].iArg[j];
        }
    }
    return acc;
}

// ---------------------------------------------------------------------------
// SubAbstatePriority -- replace/stack rules.
//
// AbState row's `Duplicate` column drives the rule:
//   Duplicate == 0 -> replace existing instance with new one
//   Duplicate == 1 -> both rows live concurrently (no replace, no merge)
//
// `StateGrade` is used as a tie-breaker for the SAME family when a
// higher-grade row tries to overwrite a lower-grade row already on the
// target (e.g. StaPoison_Lv1 vs StaPoison_Lv2). Higher grade always
// wins regardless of Duplicate flag.
// ---------------------------------------------------------------------------
bool SubAbstatePriority::ShouldReplace(uint32 uiNewAbId, uint32 uiOldAbId) {
    const AbStateRow* pkN = AbnormalStateDictionary::Get().GetRow(uiNewAbId);
    const AbStateRow* pkO = AbnormalStateDictionary::Get().GetRow(uiOldAbId);
    if (!pkN || !pkO) return uiNewAbId >= uiOldAbId;
    // Same row id: replace by default (refresh duration).
    if (uiNewAbId == uiOldAbId) {
        return pkN->uiDuplicate == 0;
    }
    // Higher StateGrade always wins.
    if (pkN->uiStateGrade != pkO->uiStateGrade)
        return pkN->uiStateGrade > pkO->uiStateGrade;
    // Same family, same grade, Duplicate=0 -> replace; Duplicate=1 -> coexist.
    return pkN->uiDuplicate == 0;
}

bool SubAbstatePriority::ShouldStack(uint32 uiNewAbId, uint32 /*uiOldAbId*/) {
    const AbStateRow* pkN = AbnormalStateDictionary::Get().GetRow(uiNewAbId);
    return pkN && pkN->uiDuplicate != 0;
}

} // namespace shine
