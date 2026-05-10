// Server/Zone/Tables/ItemDropTableW.cpp
// FEATURE: world-creation -- World/ItemDropTable.txt binder.
#include "ItemDropTableW.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"
#include <stdio.h>

namespace fiesta {

ItemDropTableW& ItemDropTableW::Get() { static ItemDropTableW s; return s; }

bool ItemDropTableW::Load(const std::string& rRoot) {
    m_kRows.clear(); m_kByMob.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\ItemDropTable.txt")) return false;
    const TsTable* t = f.Find("ItemGroup"); if (!t) return false;

    // FEATURE: world-creation -- column read: MapArea, MobId, MinLevel,
    // MaxLevel, AbStateCnt, MinCen, MaxCen, CenRate, TradeBoxA/B/C,
    // RateA/B/C, DrItem01..45 (+ DrItemR, UpGradeMin/Max, Rule, Num
    // each), ExcItem1..5
    char buf[16];
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        ItemDropMobRow x;
        x.kMapArea     = t->GetStr(r, "MapArea");
        x.kMobId       = t->GetStr(r, "MobId");
        if (x.kMobId.empty() || x.kMobId == "-") continue;
        x.uiMinLevel   = (uint16)t->GetInt(r, "MinLevel");
        x.uiMaxLevel   = (uint16)t->GetInt(r, "MaxLevel");
        x.uiAbStateCnt = (uint8) t->GetInt(r, "AbStateCnt");
        x.uiMinCen     = (uint32)t->GetInt(r, "MinCen");
        x.uiMaxCen     = (uint32)t->GetInt(r, "MaxCen");
        x.uiCenRate    = (uint32)t->GetInt(r, "CenRate");
        x.kTradeBoxA   = t->GetStr(r, "TradeBoxA");
        x.kTradeBoxB   = t->GetStr(r, "TradeBoxB");
        x.kTradeBoxC   = t->GetStr(r, "TradeBoxC");
        x.uiRateA      = (uint32)t->GetInt(r, "RateA");
        x.uiRateB      = (uint32)t->GetInt(r, "RateB");
        x.uiRateC      = (uint32)t->GetInt(r, "RateC");
        for (int s = 0; s < 45; ++s) {
            sprintf_s(buf, sizeof(buf), "DrItem%d",  s + 1);
            x.aSlots[s].kItem        = t->GetStr(r, buf);
            sprintf_s(buf, sizeof(buf), "DrItem%dR", s + 1);
            x.aSlots[s].uiRate       = (uint32)t->GetInt(r, buf);
            sprintf_s(buf, sizeof(buf), "UpGradeMin%02d", s + 1);
            x.aSlots[s].uiUpgradeMin = (uint16)t->GetInt(r, buf);
            sprintf_s(buf, sizeof(buf), "UpGradeMax%02d", s + 1);
            x.aSlots[s].uiUpgradeMax = (uint16)t->GetInt(r, buf);
            sprintf_s(buf, sizeof(buf), "Rule%d", s + 1);
            std::string ru = t->GetStr(r, buf);
            x.aSlots[s].cRule = ru.empty() ? '-' : ru[0];
            sprintf_s(buf, sizeof(buf), "Num%d", s + 1);
            x.aSlots[s].iNum = (int32)t->GetInt(r, buf);
        }
        for (int s = 0; s < 5; ++s) {
            sprintf_s(buf, sizeof(buf), "ExcItem%d", s + 1);
            x.aExcItem[s] = t->GetStr(r, buf);
        }
        m_kByMob[x.kMobId].push_back(m_kRows.size());
        m_kRows.push_back(x);
    }
    SHINELOG_INFO("ItemDropTable.txt: %u mob-drop rows", (uint32)m_kRows.size());
    return true;
}

void ItemDropTableW::RowsForMob(const std::string& rMobInx,
                                std::vector<const ItemDropMobRow*>& rOut) const {
    rOut.clear();
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kByMob.find(rMobInx);
    if (it == m_kByMob.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i)
        rOut.push_back(&m_kRows[it->second[i]]);
}

} // namespace fiesta
