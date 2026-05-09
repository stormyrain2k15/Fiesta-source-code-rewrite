// Server/Zone/ChargedEffect.cpp
#include "ChargedEffect.h"
#include "ShineObject.h"
#include "../DataReader/ShnFile.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace fiesta {

namespace {
    inline uint32 ColU32(const std::vector<ShnValue>& r, size_t i) {
        return (i < r.size()) ? (uint32)r[i].iVal : 0u;
    }
    inline const std::string& ColStr(const std::vector<ShnValue>& r, size_t i) {
        static std::string s_empty;
        return (i < r.size()) ? r[i].kStr : s_empty;
    }
}

// =============================================================================
//  ChargedEffectTable
// =============================================================================
ChargedEffectTable& ChargedEffectTable::Get() {
    static ChargedEffectTable s; return s;
}

bool ChargedEffectTable::Load(const std::string& rRoot) {
    ShnFile f;
    if (!f.LoadFromFile(rRoot + "\\ChargedEffect.shn")) {
        SHINELOG_WARN("ChargedEffect.shn load failed -- charged items disabled");
        return false;
    }
    m_kRows.clear();
    m_kByItem.clear();
    const std::vector<std::vector<ShnValue> >& rows = f.Rows();
    m_kRows.reserve(rows.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        const std::vector<ShnValue>& r = rows[i];
        ChargedEffectRow rec;
        rec.uiHandle         = (uint16)ColU32(r, 0);
        rec.kItemID          =          ColStr(r, 1);
        rec.uiKeepTime_Hour  = (uint16)ColU32(r, 2);
        rec.eEffect          = (eChargedEffect)ColU32(r, 3);
        rec.uiEffectValue    = (uint16)ColU32(r, 4);
        rec.uiStaStrength    = (uint8) ColU32(r, 5);
        m_kByItem[rec.kItemID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ChargedEffect.shn: %u rows loaded", (uint32)m_kRows.size());
    return true;
}

const ChargedEffectRow* ChargedEffectTable::FindByItem(const std::string& rItemID) const {
    std::map<std::string, size_t>::const_iterator it = m_kByItem.find(rItemID);
    return (it == m_kByItem.end()) ? NULL : &m_kRows[it->second];
}

// =============================================================================
//  ChargedEffectManager
// =============================================================================
ChargedEffectManager& ChargedEffectManager::Get() {
    static ChargedEffectManager s; return s;
}

bool ChargedEffectManager::LoadDeletable(const std::string& rRoot) {
    ShnFile f;
    if (!f.LoadFromFile(rRoot + "\\ChargedDeletableBuff.shn")) return false;
    m_kDeletable.clear();
    const std::vector<std::vector<ShnValue> >& rows = f.Rows();
    m_kDeletable.reserve(rows.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        m_kDeletable.push_back((uint16)ColU32(rows[i], 0));
    }
    SHINELOG_INFO("ChargedDeletableBuff.shn: %u handles", (uint32)m_kDeletable.size());
    return true;
}

bool ChargedEffectManager::IsDeletable(uint16 uiHandle) const {
    for (size_t i = 0; i < m_kDeletable.size(); ++i) {
        if (m_kDeletable[i] == uiHandle) return true;
    }
    return false;
}

bool ChargedEffectManager::OnItemReceived(ShinePlayer* pkP, const std::string& rItemID) {
    if (!pkP) return false;
    const ChargedEffectRow* pkRow = ChargedEffectTable::Get().FindByItem(rItemID);
    if (!pkRow) return false;

    ChargedEffectInst kInst;
    kInst.uiHandle      = pkRow->uiHandle;
    kInst.eEffect       = pkRow->eEffect;
    kInst.uiEffectValue = pkRow->uiEffectValue;
    kInst.uiExpireMs    = GTimer::NowMillis() +
                          (uint64)pkRow->uiKeepTime_Hour * 3600ULL * 1000ULL;
    kInst.bDeletable    = IsDeletable(pkRow->uiHandle);

    m_kActive[pkP->GetCharID()].push_back(kInst);
    SHINELOG_INFO("ChargedEffect: cid=%u item='%s' effect=%d val=%u keep=%uh",
                  pkP->GetCharID(), rItemID.c_str(), (int)kInst.eEffect,
                  (uint32)kInst.uiEffectValue, (uint32)pkRow->uiKeepTime_Hour);
    return true;
}

bool ChargedEffectManager::TryDelete(ShinePlayer* pkP, uint16 uiHandle) {
    if (!pkP) return false;
    std::map<CharID, std::vector<ChargedEffectInst> >::iterator it
        = m_kActive.find(pkP->GetCharID());
    if (it == m_kActive.end()) return false;
    std::vector<ChargedEffectInst>& v = it->second;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i].uiHandle == uiHandle && v[i].bDeletable) {
            v.erase(v.begin() + i);
            return true;
        }
    }
    return false;
}

void ChargedEffectManager::Tick() {
    uint64 now = GTimer::NowMillis();
    for (std::map<CharID, std::vector<ChargedEffectInst> >::iterator it = m_kActive.begin();
         it != m_kActive.end(); ++it) {
        std::vector<ChargedEffectInst>& v = it->second;
        size_t w = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i].uiExpireMs > now) {
                if (w != i) v[w] = v[i];
                ++w;
            }
        }
        if (w != v.size()) {
            SHINELOG_INFO("ChargedEffect: cid=%u expired %u of %u",
                          it->first, (uint32)(v.size() - w), (uint32)v.size());
            v.resize(w);
        }
    }
}

} // namespace fiesta
