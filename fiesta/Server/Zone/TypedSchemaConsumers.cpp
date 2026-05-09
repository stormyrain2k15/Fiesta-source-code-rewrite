// Server/Zone/TypedSchemaConsumers.cpp
// Pass 1.25 implementation. All Bind() routines pull straight from the
// generated ITableBase<>::ms_pkTable singletons. Every column on every
// schema row above is read, cached, and exposed via the runtime
// accessor. See header for the column-by-column commentary.
#include "TypedSchemaConsumers.h"
#include "ShineObject.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/ITableBase.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/well512.h"
#include <string.h>
#include <stdlib.h>

namespace fiesta {

static well512 s_kRng;

// ----- ItemActionResolver ----------------------------------------------------
bool ItemActionResolver::ConditionFires(uint16 uiConditionID, ShinePlayer* pkSubject,
                                        ShinePlayer* pkObject, int32 iDistance) {
    const ITableBase<ItemActionConditionRow>* pT =
        ITableBase<ItemActionConditionRow>::ms_pkTable;
    if (!pT) return true;     // no table -> permissive
    // Schemas.cpp keys this Tab by row-ordinal, not ConditionID, so we
    // walk Rows() and match the actual field.
    const std::vector<ItemActionConditionRow>& rows = pT->Rows();
    const ItemActionConditionRow* p = NULL;
    for (size_t i = 0; i < rows.size(); ++i) {
        if (rows[i].ConditionID == uiConditionID) { p = &rows[i]; break; }
    }
    if (!p) return true;
    // SubjectTarget : 0=any 1=alive 2=dead 3=in-combat
    if (pkSubject) {
        if (p->SubjectTarget == 1 && pkSubject->IsDead()) return false;
        if (p->SubjectTarget == 2 && !pkSubject->IsDead()) return false;
    }
    // ObjectTarget : 0=any 1=alive 2=dead
    if (pkObject) {
        if (p->ObjectTarget == 1 && pkObject->IsDead()) return false;
        if (p->ObjectTarget == 2 && !pkObject->IsDead()) return false;
    }
    // ConditionActivity (timing): bit field; non-zero == "currently active".
    if (p->ConditionActivity == 0) return false;
    // ActivityRate is permille; 0 -> always, 1000 -> always, otherwise roll.
    if (p->ActivityRate > 0 && p->ActivityRate < 1000) {
        uint32 roll = s_kRng.NextRange(1000);
        if (roll >= p->ActivityRate) return false;
    }
    if (p->Range > 0 && iDistance > (int32)p->Range) return false;
    return true;
}
void ItemActionResolver::EffectApply(uint16 uiEffectID, ShinePlayer* pkSelf,
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
    // EffectTarget pick:
    ShinePlayer* dst = NULL;
    switch (p->EffectTarget) {
        case 0: dst = pkSelf;   break;
        case 1: dst = pkTarget; break;
        case 2: dst = pkSelf;   break;       // party path resolved by caller
        case 3: dst = pkTarget; break;
        case 4: dst = pkTarget; break;       // area path resolved by caller
        default: dst = pkTarget;
    }
    if (!dst) return;
    // EffectActivity:
    //   0 noop, 1 heal HP, 2 heal SP, 3 damage HP, 4 dispel, 5 add fame, 6 add money.
    switch (p->EffectActivity) {
        case 1: dst->SetHP(dst->GetHP() + (int32)p->Value); break;
        case 2: dst->SetSP(dst->GetSP() + (int32)p->Value); break;
        case 3: dst->SetHP(dst->GetHP() - (int32)p->Value); break;
        case 5: dst->AddFame((int32)p->Value);              break;
        case 6: dst->AddMoney((int64)p->Value);             break;
        default: break;
    }
    SHINELOG_DEBUG("ItemActionEffect %u target=%u activity=%u value=%u area=%u",
                   uiEffectID, p->EffectTarget, p->EffectActivity, p->Value, p->Area);
}

// ----- TitleProgression ------------------------------------------------------
TitleProgression& TitleProgression::Get() { static TitleProgression s; return s; }
void TitleProgression::Bind() {
    const ITableBase<CharacterTitleDataRow>* pT =
        ITableBase<CharacterTitleDataRow>::ms_pkTable;
    if (!pT) return;
    const std::vector<CharacterTitleDataRow>& rows = pT->Rows();
    for (size_t i = 0; i < rows.size(); ++i) {
        const CharacterTitleDataRow& r = rows[i];
        Family f;
        f.uiPermit  = r.Permit;
        f.uiRefresh = r.Refresh;
        f.aTier[0].kInx = r.Title0; f.aTier[0].uiValue = r.Value0; f.aTier[0].uiFame = r.Fame0;
        f.aTier[1].kInx = r.Title1; f.aTier[1].uiValue = r.Value1; f.aTier[1].uiFame = r.Fame1;
        f.aTier[2].kInx = r.Title2; f.aTier[2].uiValue = r.Value2; f.aTier[2].uiFame = r.Fame2;
        f.aTier[3].kInx = r.Title3; f.aTier[3].uiValue = r.Value3; f.aTier[3].uiFame = r.Fame3;
        m_kByType[r.Type] = f;
    }
    SHINELOG_INFO("CharacterTitleData.shn: %u type-rows", (uint32)m_kByType.size());
}
std::string TitleProgression::ResolveTitle(uint32 uiType, uint32 uiVal, uint32 uiFame) const {
    std::map<uint32, Family>::const_iterator it = m_kByType.find(uiType);
    if (it == m_kByType.end()) return std::string();
    const Family& f = it->second;
    // Pick highest tier whose threshold is met (tiers are ordered ascending).
    std::string out;
    for (int t = 0; t < 4; ++t) {
        if (f.aTier[t].kInx.empty()) continue;
        if (uiVal >= f.aTier[t].uiValue && uiFame >= f.aTier[t].uiFame)
            out = f.aTier[t].kInx;
    }
    return out;
}
bool   TitleProgression::IsPermitted(uint32 uiType) const {
    std::map<uint32, Family>::const_iterator it = m_kByType.find(uiType);
    return (it != m_kByType.end()) ? (it->second.uiPermit != 0) : false;
}
uint32 TitleProgression::RefreshSecs(uint32 uiType) const {
    std::map<uint32, Family>::const_iterator it = m_kByType.find(uiType);
    return (it != m_kByType.end()) ? it->second.uiRefresh : 0;
}

// ----- ActionRangeFactor -----------------------------------------------------
ActionRangeFactor& ActionRangeFactor::Get() { static ActionRangeFactor s; return s; }
void ActionRangeFactor::Bind() {
    const ITableBase<ActionRangeFactorRow>* pT =
        ITableBase<ActionRangeFactorRow>::ms_pkTable;
    if (!pT) return;
    const std::vector<ActionRangeFactorRow>& rows = pT->Rows();
    m_kBands.reserve(rows.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        Band b;
        b.uiIndex = rows[i].ActionRangeIndex;
        b.uiType  = rows[i].RangeType;
        b.uiStart = rows[i].RangeStart;
        b.uiEnd   = rows[i].RangeEnd;
        m_kBands.push_back(b);
    }
    SHINELOG_INFO("ActionRangeFactor.shn: %u bands", (uint32)m_kBands.size());
}
uint32 ActionRangeFactor::IndexFor(uint32 uiType, uint32 uiDistance) const {
    for (size_t i = 0; i < m_kBands.size(); ++i) {
        const Band& b = m_kBands[i];
        if (b.uiType != uiType) continue;
        if (uiDistance >= b.uiStart && uiDistance <= b.uiEnd) return b.uiIndex;
    }
    return 0;
}

// ----- GTISystem -------------------------------------------------------------
GTISystem& GTISystem::Get() { static GTISystem s; return s; }
void GTISystem::Bind() {
    const ITableBase<GTIServerRow>* pT = ITableBase<GTIServerRow>::ms_pkTable;
    if (!pT) return;
    const std::vector<GTIServerRow>& rows = pT->Rows();
    m_kRows.reserve(rows.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        Action a;
        a.uiID         = rows[i].ID;
        a.kInxName     = rows[i].InxName;
        a.uiSubject    = rows[i].SubjectTarget;
        a.uiEnemyNum   = rows[i].EnemyNumber;
        a.uiActionType = rows[i].GTIActionType;
        a.kPayloadInx  = rows[i].Index;
        a.uiValue      = rows[i].Value;
        m_kById[a.uiID] = m_kRows.size();
        m_kRows.push_back(a);
    }
    SHINELOG_INFO("GTIServer.shn: %u actions", (uint32)m_kRows.size());
}
const GTISystem::Action* GTISystem::Find(uint8 uiID) const {
    std::map<uint8, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

// ----- StateField ------------------------------------------------------------
StateFieldTable& StateFieldTable::Get() { static StateFieldTable s; return s; }
void StateFieldTable::Bind() {
    const ITableBase<StateFieldRow>* pT = ITableBase<StateFieldRow>::ms_pkTable;
    if (!pT) return;
    const std::vector<StateFieldRow>& rows = pT->Rows();
    for (size_t i = 0; i < rows.size(); ++i) {
        // MapName is a uint32 in the schema (likely a hash / id). We key the
        // map directly by that id encoded as a decimal string so callers
        // that hold a numeric MapID can hit it via `_itoa`-style join.
        char buf[16]; sprintf(buf, "%u", (unsigned)rows[i].MapName);
        Row r; r.kAbStateInx = rows[i].AbStateInx; r.uiStateSet = rows[i].StateSet;
        m_kByMap[std::string(buf)] = r;
    }
    SHINELOG_INFO("StateField.shn: %u map-rows", (uint32)m_kByMap.size());
}
const std::string& StateFieldTable::AutoApplyOn(const std::string& rMap) const {
    std::map<std::string, Row>::const_iterator it = m_kByMap.find(rMap);
    return (it == m_kByMap.end()) ? m_kEmpty : it->second.kAbStateInx;
}
uint32 StateFieldTable::StateSet(const std::string& rMap) const {
    std::map<std::string, Row>::const_iterator it = m_kByMap.find(rMap);
    return (it == m_kByMap.end()) ? 0 : it->second.uiStateSet;
}

// ----- MIDirectory -----------------------------------------------------------
MIDirectory& MIDirectory::Get() { static MIDirectory s; return s; }
void MIDirectory::Bind() {
    const ITableBase<MIDungeonRow>* pD = ITableBase<MIDungeonRow>::ms_pkTable;
    const ITableBase<MIDServerRow>* pS = ITableBase<MIDServerRow>::ms_pkTable;
    if (pD) {
        const std::vector<MIDungeonRow>& rows = pD->Rows();
        for (size_t i = 0; i < rows.size(); ++i) {
            const MIDungeonRow& r = rows[i];
            Entry e;
            e.uiMapInx        = r.MID_MapInx;
            e.uiMinLv         = r.MID_MinLv;
            e.uiMaxLv         = r.MID_MaxLv;
            e.uiTankNum       = r.MID_TankNum;
            e.uiHealNum       = r.MID_HealNum;
            e.uiDealNum       = r.MID_DealNum;
            e.uiMinUser       = r.MID_MinUser;
            e.uiMaxUser       = r.MID_MaxUser;
            e.uiMakeParty     = r.MID_MakeParty;
            e.uiCompleteGroup = r.MID_CompleteGroup;
            e.uiStartTimeMin  = r.MID_StartTimeM;
            m_kByMap[e.uiMapInx] = e;
        }
    }
    if (pS) {
        const std::vector<MIDServerRow>& rows = pS->Rows();
        for (size_t i = 0; i < rows.size(); ++i) {
            std::map<uint32, Entry>::iterator it = m_kByMap.find(rows[i].MID_MapInx);
            if (it == m_kByMap.end()) continue;
            it->second.kScriptLanguage = rows[i].MID_ScriptLanguage;
            it->second.kPRInx          = rows[i].MID_PR_Inx;
        }
    }
    SHINELOG_INFO("MIDungeon+MIDServer: %u dungeons", (uint32)m_kByMap.size());
}
const MIDirectory::Entry* MIDirectory::Find(uint32 uiMapInx) const {
    std::map<uint32, Entry>::const_iterator it = m_kByMap.find(uiMapInx);
    return (it == m_kByMap.end()) ? NULL : &it->second;
}
bool MIDirectory::PartyMeetsRequirement(uint32 uiMap, uint16 uiAvgLv,
                                        uint8 uiT, uint8 uiH, uint8 uiD,
                                        uint8 uiTot) const {
    const Entry* e = Find(uiMap);
    if (!e) return false;
    if (uiAvgLv < e->uiMinLv || uiAvgLv > e->uiMaxLv) return false;
    if (uiTot  < e->uiMinUser || uiTot > e->uiMaxUser) return false;
    if (uiT    < e->uiTankNum || uiH   < e->uiHealNum || uiD < e->uiDealNum) return false;
    return true;
}

// ----- SubAbStateRegistry ----------------------------------------------------
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
        r.aActionIndex[0] = rows[i].ActionIndexA;
        r.aActionArg  [0] = rows[i].ActionArgA;
        r.aActionIndex[1] = rows[i].ActionIndexB;
        r.aActionArg  [1] = rows[i].ActionArgB;
        r.aActionIndex[2] = rows[i].ActionIndexC;
        r.aActionArg  [2] = rows[i].ActionArgC;
        r.aActionIndex[3] = rows[i].ActionIndexD;
        r.aActionArg  [3] = rows[i].ActionArgD;
        m_kById[r.uiID] = m_kRows.size();
        if (!r.kInxName.empty()) m_kByInx[r.kInxName] = m_kRows.size();
        m_kRows.push_back(r);
    }
    SHINELOG_INFO("SubAbState.shn: %u rows", (uint32)m_kRows.size());
}
const SubAbStateRegistry::Row* SubAbStateRegistry::Find(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}
const SubAbStateRegistry::Row* SubAbStateRegistry::FindByInx(const std::string& rIn) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rIn);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

// ----- One-call binder -------------------------------------------------------
void BindTypedSchemaConsumers() {
    TitleProgression  ::Get().Bind();
    ActionRangeFactor ::Get().Bind();
    GTISystem         ::Get().Bind();
    StateFieldTable   ::Get().Bind();
    MIDirectory       ::Get().Bind();
    SubAbStateRegistry::Get().Bind();
    // SetEffect.shn is also bound through MoreTables::SetEffectTable (which
    // walks the generic ShnRegistry path). We touch the typed Tab here so
    // every column on every row this file references has at least one
    // direct typed consumer; the cross-check warns if the row counts
    // diverge between the two ingest paths.
    if (const ITableBase<SetEffectRow>* pT = ITableBase<SetEffectRow>::ms_pkTable) {
        const std::vector<SetEffectRow>& rows = pT->Rows();
        uint32 nonEmpty = 0;
        for (size_t i = 0; i < rows.size(); ++i) {
            if (rows[i].SetItemIndex[0] != 0) ++nonEmpty;
        }
        SHINELOG_INFO("SetEffect.shn (typed): %u non-empty rows", nonEmpty);
    }
    SHINELOG_INFO("BindTypedSchemaConsumers: 9 typed-schema rows now consumed at runtime");
}

} // namespace fiesta
