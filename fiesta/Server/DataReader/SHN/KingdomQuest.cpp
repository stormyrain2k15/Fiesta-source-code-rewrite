// Server/DataReader/SHN/KingdomQuest.cpp
// Auto-generated: one-file-per-SHN split for KingdomQuest.shn
#include "KingdomQuest.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

KingdomQuestShn& KingdomQuestShn::Get() { static KingdomQuestShn s; return s; }

void KingdomQuestShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuest");
    if (!t) { SHINELOG_WARN("KingdomQuest.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        KingdomQuestRow rec;
        rec.iID = (int16)ShnGetI32(*t, _r, "ID");
        rec.kTitle = ShnGetStr(*t, _r, "Title");
        rec.uiLimitTime = (uint16)ShnGetU32(*t, _r, "LimitTime");
        rec.uiST_Year = (uint8)ShnGetU32(*t, _r, "ST_Year");
        rec.uiST_Month = (uint8)ShnGetU32(*t, _r, "ST_Month");
        rec.uiST_Day = (uint8)ShnGetU32(*t, _r, "ST_Day");
        rec.uiST_Hour = (uint8)ShnGetU32(*t, _r, "ST_Hour");
        rec.uiST_Minute = (uint8)ShnGetU32(*t, _r, "ST_Minute");
        rec.uiST_Second = (uint8)ShnGetU32(*t, _r, "ST_Second");
        rec.uiStartWaitTime = (uint16)ShnGetU32(*t, _r, "StartWaitTime");
        rec.uiNextStartMode = (uint8)ShnGetU32(*t, _r, "NextStartMode");
        rec.uiNextStartDeleyMin = (uint16)ShnGetU32(*t, _r, "NextStartDeleyMin");
        rec.uiRepeatMode = (uint8)ShnGetU32(*t, _r, "RepeatMode");
        rec.uiRepeatCount = (uint16)ShnGetU32(*t, _r, "RepeatCount");
        rec.uiMinLevel = (uint8)ShnGetU32(*t, _r, "MinLevel");
        rec.uiMaxLevel = (uint8)ShnGetU32(*t, _r, "MaxLevel");
        rec.uiMinPlayers = (uint16)ShnGetU32(*t, _r, "MinPlayers");
        rec.uiMaxPlayers = (uint16)ShnGetU32(*t, _r, "MaxPlayers");
        rec.uiPlayerRepeatMode = (uint8)ShnGetU32(*t, _r, "PlayerRepeatMode");
        rec.uiPlayerRepeatCount = (uint16)ShnGetU32(*t, _r, "PlayerRepeatCount");
        rec.uiPlayerRevivalMode = (uint8)ShnGetU32(*t, _r, "PlayerRevivalMode");
        rec.uiPlayerRevivalCount = (uint8)ShnGetU32(*t, _r, "PlayerRevivalCount");
        rec.uiDemandQuest = (uint16)ShnGetU32(*t, _r, "DemandQuest");
        rec.uiDemandItem = (uint16)ShnGetU32(*t, _r, "DemandItem");
        rec.uiDemandMobKill = (uint8)ShnGetU32(*t, _r, "DemandMobKill");
        rec.uiRewardIndex = ShnGetU32(*t, _r, "RewardIndex");
        rec.iMapLink = (int16)ShnGetI32(*t, _r, "MapLink");
        rec.iUnkCol27 = (int16)ShnGetI32(*t, _r, "UnkCol27");
        rec.iUnkCol28 = (int16)ShnGetI32(*t, _r, "UnkCol28");
        rec.iUnkCol29 = (int16)ShnGetI32(*t, _r, "UnkCol29");
        rec.kScriptLanguage = ShnGetStr(*t, _r, "ScriptLanguage");
        rec.kInitValue = ShnGetStr(*t, _r, "InitValue");
        rec.uiUseClass = ShnGetU32(*t, _r, "UseClass");
        rec.iDemandGender = (int8)ShnGetI32(*t, _r, "DemandGender");
        rec.iUnkCol34 = (int8)ShnGetI32(*t, _r, "UnkCol34");
        m_kById[rec.iID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("KingdomQuest.shn: %u rows", (uint32)m_kRows.size());
}

const KingdomQuestRow* KingdomQuestShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
