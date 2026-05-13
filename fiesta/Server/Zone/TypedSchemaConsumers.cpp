// Server/Zone/TypedSchemaConsumers.cpp
#include "TypedSchemaConsumers.h"
#include "ShineObject.h"
#include "AbState.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/ITableBase.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/well512.h"
#include <string.h>
#include <stdlib.h>

namespace shine {

static well512 s_kRng;

// ---------------------------------------------------------------------------
// ItemActionResolver
// The two halves of an ItemAction row (Condition + Effect) are the
// authoritative gate + payload for any item whose ItemFunc points at an
// ItemAction.  ConditionFires evaluates SubjectTarget / ObjectTarget /
// ConditionActivity / ActivityRate / Range; EffectApply mutates HP / SP /
// Fame / Money on the chosen target according to EffectActivity.
// ---------------------------------------------------------------------------
bool ItemActionResolver::ConditionFires(uint16 uiConditionID,
                                        ShinePlayer* pkSubject,
                                        ShinePlayer* pkObject,
                                        int32 iDistance) {
    const ITableBase<ItemActionConditionRow>* pT =
        ITableBase<ItemActionConditionRow>::ms_pkTable;
    if (!pT) return true;
    const std::vector<ItemActionConditionRow>& rows = pT->Rows();
    const ItemActionConditionRow* p = NULL;
    for (size_t i = 0; i < rows.size(); ++i) {
        if (rows[i].ConditionID == uiConditionID) { p = &rows[i]; break; }
    }
    if (!p) return true;
    if (pkSubject) {
        if (p->SubjectTarget == 1 && pkSubject->IsDead())   return false;
        if (p->SubjectTarget == 2 && !pkSubject->IsDead())  return false;
    }
    if (pkObject) {
        if (p->ObjectTarget == 1 && pkObject->IsDead())     return false;
        if (p->ObjectTarget == 2 && !pkObject->IsDead())    return false;
    }
    if (p->ConditionActivity == 0) return false;
    if (p->ActivityRate > 0 && p->ActivityRate < 1000) {
        if (s_kRng.NextRange(1000) >= p->ActivityRate) return false;
    }
    if (p->Range > 0 && iDistance > (int32)p->Range) return false;
    return true;
}

void ItemActionResolver::EffectApply(uint16 uiEffectID,
                                     ShinePlayer* pkSelf,
                                     ShinePlayer* pkTarget) {
    const ITableBase<ItemActionEffectRow>* pT =
        ITableBase<ItemActionEffectRow>::ms_pkTable;
    if (!pT) return;
    const std::vector<ItemActionEffectRow>& erows = pT->Rows();
    const ItemActionEffectRow* p = NULL;
    for (size_t i = 0; i < erows.size(); ++i) {
        if (erows[i].EffectID == uiEffectID) { p = &erows[i]; break; }
    }
    if (!p) return;
    ShinePlayer* dst = NULL;
    switch (p->EffectTarget) {
        case 0: dst = pkSelf;   break;
        case 1: dst = pkTarget; break;
        case 2: dst = pkSelf;   break;
        case 3: dst = pkTarget; break;
        case 4: dst = pkTarget; break;
        default: dst = pkTarget;
    }
    if (!dst) return;
    switch (p->EffectActivity) {
        case 1: dst->SetHP   (dst->GetHP() + (int32)p->Value); break;
        case 2: dst->SetSP   (dst->GetSP() + (int32)p->Value); break;
        case 3: dst->SetHP   (dst->GetHP() - (int32)p->Value); break;
        case 4: {
            // EffectActivity=4 is the AbState apply path. The schema's
            // numeric `Value` indexes the SubAbState row id we want to
            // fire on `dst`. Strength is taken from `Area` (this column
            // doubles as the strength tier on AbState-class effects);
            // duration falls back to the SubAbState row's own KeepTime.
            const SubAbStateRegistry::Row* pkSub =
                SubAbStateRegistry::Get().Find((uint32)p->Value);
            if (!pkSub) {
                SHINELOG_WARN("ItemAction effect=%u: SubAbState id=%u not found",
                              uiEffectID, (uint32)p->Value);
                break;
            }
            dst->AbState().ApplySubByName(pkSub->kInxName.c_str(),
                                          p->Area > 0 ? (uint32)p->Area : 1, 0);
            break;
        }
        case 5: dst->AddFame ((int32)p->Value);                break;
        case 6: dst->AddMoney((int64)p->Value);                break;
        default:
            // Unknown EffectActivity -- log so a real capture surfaces
            // the missing handler. Never silently no-op.
            SHINELOG_WARN("ItemAction effect=%u: unhandled EffectActivity=%u "
                          "value=%u area=%u (add a case to "
                          "ItemActionResolver::EffectApply)",
                          uiEffectID, (uint32)p->EffectActivity,
                          (uint32)p->Value, (uint32)p->Area);
            break;
    }
}

// ---------------------------------------------------------------------------
// StateFieldTable
// Per-map auto-applied AbState. ZoneServer calls OnPlayerEnter() the
// moment a character lands on a Field, so the world-builder side of the
// row (StateField.AbStateInx + StateSet) drives a real AbnormalState
// mutation.
// ---------------------------------------------------------------------------
StateFieldTable& StateFieldTable::Get() { static StateFieldTable s; return s; }

void StateFieldTable::Bind() {
    const ITableBase<StateFieldRow>* pT = ITableBase<StateFieldRow>::ms_pkTable;
    if (!pT) return;
    const std::vector<StateFieldRow>& rows = pT->Rows();
    for (size_t i = 0; i < rows.size(); ++i) {
        char buf[16]; sprintf(buf, "%u", (unsigned)rows[i].MapName);
        Row r; r.kAbStateInx = rows[i].AbStateInx; r.uiStateSet = rows[i].StateSet;
        m_kByMap[std::string(buf)] = r;
    }
    SHINELOG_INFO("StateField: %u map auto-states loaded", (uint32)m_kByMap.size());
}

void StateFieldTable::OnPlayerEnter(ShinePlayer* pk, const std::string& rMap) const {
    if (!pk) return;
    std::map<std::string, Row>::const_iterator it = m_kByMap.find(rMap);
    if (it == m_kByMap.end()) return;
    if (it->second.kAbStateInx.empty()) return;
    // The map row carries a SubAbState InxName (per the schema). Route
    // through the SubAbState-direct apply path with a strength of 1
    // (StateField rows don't expose a Strength column). Duration is the
    // SubAbState KeepTime; long-lived field auras are kept alive by
    // the per-tick refresh path that re-applies on each Field re-entry.
    if (pk->AbState().ApplySubByName(it->second.kAbStateInx.c_str(), 1, 0)) {
        SHINELOG_DEBUG("StateField: applied %s on map %s (cid=%u)",
                       it->second.kAbStateInx.c_str(), rMap.c_str(),
                       (uint32)pk->GetCharID());
    }
}

// ---------------------------------------------------------------------------
// SubAbStateRegistry
// Looked up by `Skill::TryUse` (StaApply slots) and by `StateFieldTable`
// to resolve an AbState inx-name to the numeric id required by
// AbnormalState::Apply along with its KeepTime/Strength.
// ---------------------------------------------------------------------------
SubAbStateRegistry& SubAbStateRegistry::Get() { static SubAbStateRegistry s; return s; }

void SubAbStateRegistry::Bind() {
    const ITableBase<SubAbStateRow>* pT = ITableBase<SubAbStateRow>::ms_pkTable;
    if (!pT) return;
    const std::vector<SubAbStateRow>& rows = pT->Rows();
    m_kRows.reserve(rows.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        Row r;
        r.uiID         = rows[i].ID;
        r.kInxName     = rows[i].InxName;
        r.uiStrength   = rows[i].Strength;
        r.uiType       = rows[i].Type;
        r.uiSubType    = rows[i].SubType;
        r.uiKeepTimeMs = rows[i].KeepTime;
        r.aActionIndex[0] = rows[i].ActionIndexA; r.aActionArg[0] = rows[i].ActionArgA;
        r.aActionIndex[1] = rows[i].ActionIndexB; r.aActionArg[1] = rows[i].ActionArgB;
        r.aActionIndex[2] = rows[i].ActionIndexC; r.aActionArg[2] = rows[i].ActionArgC;
        r.aActionIndex[3] = rows[i].ActionIndexD; r.aActionArg[3] = rows[i].ActionArgD;
        m_kById[r.uiID] = m_kRows.size();
        if (!r.kInxName.empty()) m_kByInx[r.kInxName] = m_kRows.size();
        m_kRows.push_back(r);
    }
    SHINELOG_INFO("SubAbState: %u rows", (uint32)m_kRows.size());
}

const SubAbStateRegistry::Row* SubAbStateRegistry::Find(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}
const SubAbStateRegistry::Row* SubAbStateRegistry::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

void BindTypedSchemaConsumers() {
    StateFieldTable   ::Get().Bind();
    SubAbStateRegistry::Get().Bind();
}

} // namespace shine
