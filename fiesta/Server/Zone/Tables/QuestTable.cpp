// Server/Zone/Tables/QuestTable.cpp
// FEATURE: world-creation -- World/Quest.txt binder.
// Six logical sub-tables share one TableScriptFile: Header, Reward,
// Looting, Hunting, Produce, Meeting. The runtime queries them
// through the Header(handle) -> sub-list pattern.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

static std::vector<QuestRewardRow>  s_kEmptyRewards;
static std::vector<QuestLootRow>    s_kEmptyLoots;
static std::vector<QuestHuntRow>    s_kEmptyHunts;
static std::vector<QuestProduceRow> s_kEmptyProduces;

QuestTable& QuestTable::Get() { static QuestTable s; return s; }

bool QuestTable::Load(const std::string& rRoot) {
    m_kHeaders.clear(); m_kRewards.clear(); m_kLoots.clear();
    m_kHunts.clear(); m_kProduces.clear(); m_kMeetings.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\Quest.txt")) return false;

    // FEATURE: world-creation -- column read: QuestHandle, Title,
    // LevLow, LevHigh, ReceiveScript, MissionScript, ProgressScript,
    // CompleteScript, StartNPC, StopNPC, TimeLimit(min)
    if (const TsTable* t = f.Find("Header")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            QuestHeader h;
            h.uiHandle        = (uint16)t->GetInt(r, "QuestHandle");
            if (h.uiHandle == 0) continue;
            h.kTitle          = t->GetStr(r, "Title");
            h.uiLevLow        = (uint16)t->GetInt(r, "LevLow");
            h.uiLevHigh       = (uint16)t->GetInt(r, "LevHigh");
            h.kReceiveScript  = t->GetStr(r, "ReceiveScript");
            h.kMissionScript  = t->GetStr(r, "MissionScript");
            h.kProgressScript = t->GetStr(r, "ProgressScript");
            h.kCompleteScript = t->GetStr(r, "CompleteScript");
            h.kStartNPC       = t->GetStr(r, "StartNPC");
            h.kStopNPC        = t->GetStr(r, "StopNPC");
            h.uiTimeLimitMin  = (uint16)t->GetInt(r, "TimeLimit(min)");
            m_kHeaders[h.uiHandle] = h;
        }
    }
    // FEATURE: world-creation -- column read: QuestHandle, Flag, Type,
    // Quantity, ItemName, ItemUpgrade
    if (const TsTable* t = f.Find("Reward")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            QuestRewardRow x;
            x.uiHandle    = (uint16)t->GetInt(r, "QuestHandle");
            x.uiFlag      = (uint8) t->GetInt(r, "Flag");
            x.kType       = t->GetStr(r, "Type");
            x.uiQuantity  = (uint32)t->GetInt(r, "Quantity");
            x.kItemName   = t->GetStr(r, "ItemName");
            x.uiItemUpgrade=(uint8) t->GetInt(r, "ItemUpgrade");
            m_kRewards[x.uiHandle].push_back(x);
        }
    }
    // FEATURE: world-creation -- column read: QuestHandle, Mob, Item,
    // Number, Permill
    if (const TsTable* t = f.Find("Looting")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            QuestLootRow x;
            x.uiHandle   = (uint16)t->GetInt(r, "QuestHandle");
            x.kMob       = t->GetStr(r, "Mob");
            x.kItem      = t->GetStr(r, "Item");
            x.uiNumber   = (uint8) t->GetInt(r, "Number");
            x.uiPermill  = (uint16)t->GetInt(r, "Permill");
            m_kLoots[x.uiHandle].push_back(x);
        }
    }
    // FEATURE: world-creation -- column read: QuestHandle, Mob, Number
    if (const TsTable* t = f.Find("Hunting")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            QuestHuntRow x;
            x.uiHandle = (uint16)t->GetInt(r, "QuestHandle");
            x.kMob     = t->GetStr(r, "Mob");
            x.uiNumber = (uint16)t->GetInt(r, "Number");
            m_kHunts[x.uiHandle].push_back(x);
        }
    }
    // FEATURE: world-creation -- column read: QuestHandle, RawItem0..3,
    // ToItem, Map, CenterX, CenterY, Region
    if (const TsTable* t = f.Find("Produce")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            QuestProduceRow x;
            x.uiHandle = (uint16)t->GetInt(r, "QuestHandle");
            x.aRawItem[0] = t->GetStr(r, "RawItem0");
            x.aRawItem[1] = t->GetStr(r, "RawItem1");
            x.aRawItem[2] = t->GetStr(r, "RawItem2");
            x.aRawItem[3] = t->GetStr(r, "RawItem3");
            x.kToItem  = t->GetStr(r, "ToItem");
            x.kMap     = t->GetStr(r, "Map");
            x.uiCenterX= (uint32)t->GetInt(r, "CenterX");
            x.uiCenterY= (uint32)t->GetInt(r, "CenterY");
            x.uiRegion = (uint32)t->GetInt(r, "Region");
            m_kProduces[x.uiHandle].push_back(x);
        }
    }
    // FEATURE: world-creation -- column read: QuestHandle
    if (const TsTable* t = f.Find("Meeting")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint16 h = (uint16)t->GetInt(r, "QuestHandle");
            if (h) m_kMeetings[h] = true;
        }
    }
    SHINELOG_INFO("Quest.txt: %u headers, %u rewards, %u loots, %u hunts, %u produces, %u meetings",
                  (uint32)m_kHeaders.size(), (uint32)m_kRewards.size(),
                  (uint32)m_kLoots.size(), (uint32)m_kHunts.size(),
                  (uint32)m_kProduces.size(), (uint32)m_kMeetings.size());
    return true;
}

const QuestHeader* QuestTable::Header(uint16 h) const {
    std::map<uint16, QuestHeader>::const_iterator it = m_kHeaders.find(h);
    return (it == m_kHeaders.end()) ? NULL : &it->second;
}
const std::vector<QuestRewardRow>& QuestTable::RewardsFor(uint16 h) const {
    std::map<uint16, std::vector<QuestRewardRow> >::const_iterator it = m_kRewards.find(h);
    return (it == m_kRewards.end()) ? s_kEmptyRewards : it->second;
}
const std::vector<QuestLootRow>& QuestTable::LootsFor(uint16 h) const {
    std::map<uint16, std::vector<QuestLootRow> >::const_iterator it = m_kLoots.find(h);
    return (it == m_kLoots.end()) ? s_kEmptyLoots : it->second;
}
const std::vector<QuestHuntRow>& QuestTable::HuntsFor(uint16 h) const {
    std::map<uint16, std::vector<QuestHuntRow> >::const_iterator it = m_kHunts.find(h);
    return (it == m_kHunts.end()) ? s_kEmptyHunts : it->second;
}
const std::vector<QuestProduceRow>& QuestTable::ProducesFor(uint16 h) const {
    std::map<uint16, std::vector<QuestProduceRow> >::const_iterator it = m_kProduces.find(h);
    return (it == m_kProduces.end()) ? s_kEmptyProduces : it->second;
}
bool QuestTable::IsMeeting(uint16 h) const {
    std::map<uint16, bool>::const_iterator it = m_kMeetings.find(h);
    return it != m_kMeetings.end();
}

} // namespace fiesta
