// Server/Zone/MiscTables.cpp
#include "MiscTables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

namespace {
    inline const ShnFile* T(const char* n) { return ShnRegistry::Get().GetTable(n); }
}

// =============================================================================
//  DamageLvGapTables
// =============================================================================
DamageLvGapTables& DamageLvGapTables::Get() { static DamageLvGapTables s; return s; }
void DamageLvGapTables::Bind() {
    if (const ShnFile* t = T("DamageLvGapPVE")) {
        m_kPvE.reserve(t->Rows().size());
        for (size_t i = 0; i < t->Rows().size(); ++i) {
            // First column is the diff key, second is the multiplier.
            m_kPvE.push_back((int32)ShnGetI32(*t, i, "Multiplier"));
        }
    }
    if (const ShnFile* t = T("DamageLvGapEVP")) {
        m_kEvP.reserve(t->Rows().size());
        for (size_t i = 0; i < t->Rows().size(); ++i) {
            m_kEvP.push_back((int32)ShnGetI32(*t, i, "Multiplier"));
        }
    }
    if (const ShnFile* t = T("DamageLvGapPVP")) {
        m_uiPvPSide = (uint32)t->Rows().size();
        m_kPvP.assign(m_uiPvPSide * m_uiPvPSide, 1000);
        // Each row has uiPvPSide columns; column header names are class
        // codes. We pull values by raw index since the first column is the
        // attacker class id and the rest are per-defender multipliers.
        for (size_t a = 0; a < m_uiPvPSide; ++a) {
            const std::vector<ShnValue>& row = t->Rows()[a];
            for (size_t d = 0; d < row.size() && d < m_uiPvPSide; ++d) {
                m_kPvP[a * m_uiPvPSide + d] = (int32)row[d].iVal;
            }
        }
    }
}
int32 DamageLvGapTables::PvE(int32 iDiff) const {
    if (m_kPvE.empty()) return 1000;
    int32 idx = iDiff + 12;     // 24-row table centred on diff=0
    if (idx < 0) idx = 0;
    if (idx >= (int32)m_kPvE.size()) idx = (int32)m_kPvE.size() - 1;
    return m_kPvE[idx];
}
int32 DamageLvGapTables::EvP(int32 iDiff) const {
    if (m_kEvP.empty()) return 1000;
    int32 idx = iDiff + 12;
    if (idx < 0) idx = 0;
    if (idx >= (int32)m_kEvP.size()) idx = (int32)m_kEvP.size() - 1;
    return m_kEvP[idx];
}
int32 DamageLvGapTables::PvP(int32 /*iDiff*/, uint32 uiClassA, uint32 uiClassD) const {
    if (m_uiPvPSide == 0) return 1000;
    if (uiClassA >= m_uiPvPSide || uiClassD >= m_uiPvPSide) return 1000;
    return m_kPvP[uiClassA * m_uiPvPSide + uiClassD];
}

// =============================================================================
//  ActionTables
// =============================================================================
ActionTables& ActionTables::Get() { static ActionTables s; return s; }
void ActionTables::Bind() {
    if (const ShnFile* t = T("ActionEffectItem")) {
        for (size_t i = 0; i < t->Rows().size(); ++i) {
            m_kEffect[ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Effect");
        }
    }
    if (const ShnFile* t = T("ActionRangeFactor")) {
        m_kRangeFactor.reserve(t->Rows().size());
        for (size_t i = 0; i < t->Rows().size(); ++i) {
            m_kRangeFactor.push_back(ShnGetI32(*t, i, "Factor"));
        }
    }
}
int32 ActionTables::ItemEffect(uint32 uiID) const {
    std::map<uint32, int32>::const_iterator it = m_kEffect.find(uiID);
    return (it == m_kEffect.end()) ? 0 : it->second;
}
int32 ActionTables::RangeFactor(uint32 uiKind) const {
    return (uiKind < m_kRangeFactor.size()) ? m_kRangeFactor[uiKind] : 1000;
}

// =============================================================================
//  StateTables
// =============================================================================
StateTables& StateTables::Get() { static StateTables s; return s; }
void StateTables::Bind() {
    if (const ShnFile* t = T("StateField"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kField[ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Effect");
    if (const ShnFile* t = T("StateItem"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kItem [ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Effect");
    if (const ShnFile* t = T("StateMob"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kMob  [ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Effect");
}
int32 StateTables::FieldEffect(uint32 uiMapID) const {
    std::map<uint32, int32>::const_iterator it = m_kField.find(uiMapID);
    return (it == m_kField.end()) ? 0 : it->second; }
int32 StateTables::ItemEffect (uint32 uiItem)  const {
    std::map<uint32, int32>::const_iterator it = m_kItem.find(uiItem);
    return (it == m_kItem.end()) ? 0 : it->second; }
int32 StateTables::MobEffect  (uint32 uiMob)   const {
    std::map<uint32, int32>::const_iterator it = m_kMob.find(uiMob);
    return (it == m_kMob.end()) ? 0 : it->second; }

// =============================================================================
//  CraftTables (TownPortal / Gather / Produce)
// =============================================================================
CraftTables& CraftTables::Get() { static CraftTables s; return s; }
void CraftTables::Bind() {
    if (const ShnFile* t = T("TownPortal"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kTP[ShnGetU32(*t, i, "ID")] = true;
    if (const ShnFile* t = T("Gather"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kGather[ShnGetU32(*t, i, "ID")] = true;
    if (const ShnFile* t = T("Produce"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kProduce[ShnGetU32(*t, i, "ID")] = true;
}
bool CraftTables::HasTownPortal(uint32 u) const { return m_kTP.find(u) != m_kTP.end(); }
bool CraftTables::HasGather    (uint32 u) const { return m_kGather.find(u) != m_kGather.end(); }
bool CraftTables::HasProduce   (uint32 u) const { return m_kProduce.find(u) != m_kProduce.end(); }

// =============================================================================
//  HPRewardTable / RidingTable
// =============================================================================
HPRewardTable& HPRewardTable::Get() { static HPRewardTable s; return s; }
void HPRewardTable::Bind() {
    if (const ShnFile* t = T("HolyPromiseReward"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kRow[ShnGetU32(*t, i, "PromiseCount")] = ShnGetI32(*t, i, "Reward");
}
int32 HPRewardTable::ForCount(uint32 uiCount) const {
    std::map<uint32, int32>::const_iterator it = m_kRow.find(uiCount);
    return (it == m_kRow.end()) ? 0 : it->second; }

RidingTable& RidingTable::Get() { static RidingTable s; return s; }
void RidingTable::Bind() {
    if (const ShnFile* t = T("Riding"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kSpeed[ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Speed");
}
int32 RidingTable::SpeedFor(uint32 uiID) const {
    std::map<uint32, int32>::const_iterator it = m_kSpeed.find(uiID);
    return (it == m_kSpeed.end()) ? 0 : it->second; }

// =============================================================================
//  BadNameFilter
// =============================================================================
BadNameFilter& BadNameFilter::Get() { static BadNameFilter s; return s; }
void BadNameFilter::Bind() {
    if (const ShnFile* t = T("BadNameFilter")) {
        m_kPatterns.reserve(t->Rows().size());
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kPatterns.push_back(ShnGetStr(*t, i, "Pattern"));
        SHINELOG_INFO("BadNameFilter: %u patterns", (uint32)m_kPatterns.size());
    }
}
bool BadNameFilter::IsBlocked(const std::string& rName) const {
    for (size_t i = 0; i < m_kPatterns.size(); ++i) {
        const std::string& p = m_kPatterns[i];
        if (p.empty()) continue;
        if (rName.find(p) != std::string::npos) return true;
    }
    return false;
}

// =============================================================================
//  One-call binder
// =============================================================================
void BindAllMiscTables() {
    DamageLvGapTables::Get().Bind();
    ActionTables::Get()     .Bind();
    StateTables::Get()      .Bind();
    CraftTables::Get()      .Bind();
    HPRewardTable::Get()    .Bind();
    RidingTable::Get()      .Bind();
    BadNameFilter::Get()    .Bind();
}

} // namespace fiesta
