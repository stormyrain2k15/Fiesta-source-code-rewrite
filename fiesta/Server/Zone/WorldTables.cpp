// Server/Zone/WorldTables.cpp
#include "WorldTables.h"
#include "ScenarioScript.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>
#include <string.h>

namespace fiesta {

static std::vector<QuestRewardRow> kEmptyRewards;
static std::vector<QuestLootRow>   kEmptyLoots;
static std::vector<QuestHuntRow>   kEmptyHunts;
static std::vector<QuestProduceRow> kEmptyProduces;

// ============================================================================
// QuestTable
// ============================================================================
QuestTable& QuestTable::Get() { static QuestTable s; return s; }
bool QuestTable::Load(const std::string& rRoot) {
    m_kHeaders.clear(); m_kRewards.clear(); m_kLoots.clear();
    m_kHunts.clear(); m_kProduces.clear(); m_kMeetings.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\Quest.txt")) return false;

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
    if (const TsTable* t = f.Find("Hunting")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            QuestHuntRow x;
            x.uiHandle = (uint16)t->GetInt(r, "QuestHandle");
            x.kMob     = t->GetStr(r, "Mob");
            x.uiNumber = (uint16)t->GetInt(r, "Number");
            m_kHunts[x.uiHandle].push_back(x);
        }
    }
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
    if (const TsTable* t = f.Find("Meeting")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint16 h = (uint16)t->GetInt(r, "QuestHandle");
            if (h) m_kMeetings[h] = true;
        }
    }
    SHINELOG_INFO("Quest: %u headers, %u rewards, %u loots, %u hunts, %u produces, %u meetings",
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
    return (it == m_kRewards.end()) ? kEmptyRewards : it->second;
}
const std::vector<QuestLootRow>& QuestTable::LootsFor(uint16 h) const {
    std::map<uint16, std::vector<QuestLootRow> >::const_iterator it = m_kLoots.find(h);
    return (it == m_kLoots.end()) ? kEmptyLoots : it->second;
}
const std::vector<QuestHuntRow>& QuestTable::HuntsFor(uint16 h) const {
    std::map<uint16, std::vector<QuestHuntRow> >::const_iterator it = m_kHunts.find(h);
    return (it == m_kHunts.end()) ? kEmptyHunts : it->second;
}
const std::vector<QuestProduceRow>& QuestTable::ProducesFor(uint16 h) const {
    std::map<uint16, std::vector<QuestProduceRow> >::const_iterator it = m_kProduces.find(h);
    return (it == m_kProduces.end()) ? kEmptyProduces : it->second;
}
bool QuestTable::IsMeeting(uint16 h) const {
    std::map<uint16, bool>::const_iterator it = m_kMeetings.find(h);
    return it != m_kMeetings.end();
}

// ============================================================================
// ExpRecalcTable
// ============================================================================
ExpRecalcTable& ExpRecalcTable::Get() { static ExpRecalcTable s; return s; }
bool ExpRecalcTable::Load(const std::string& rRoot) {
    for (int i = 0; i < 8; ++i) m_aPartyBonus[i] = 1000;
    m_kLevelDiffBonus.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\ExpRecalculation.txt")) return false;
    if (const TsTable* t = f.Find("ByPartyMem")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint8 m = (uint8)t->GetInt(r, "PartyMember");
            int32 b = (int32)t->GetInt(r, "Bonus");
            if (m < 8) m_aPartyBonus[m] = b;
        }
    }
    if (const TsTable* t = f.Find("ByLevelDiff")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            int32 d = (int32)t->GetInt(r, "LevelDiff");
            int32 b = (int32)t->GetInt(r, "Bonus");
            m_kLevelDiffBonus[d] = b;
        }
    }
    return true;
}
int32 ExpRecalcTable::PartyBonus(uint8 m) const {
    return (m < 8) ? m_aPartyBonus[m] : 1000;
}
int32 ExpRecalcTable::LevelDiffBonus(int32 d) const {
    std::map<int32, int32>::const_iterator it = m_kLevelDiffBonus.find(d);
    if (it != m_kLevelDiffBonus.end()) return it->second;
    // Find nearest below, since the file uses sparse anchor rows.
    int32 best = 1000; bool found = false;
    for (it = m_kLevelDiffBonus.begin(); it != m_kLevelDiffBonus.end(); ++it) {
        if (it->first <= d) { best = it->second; found = true; }
        else if (!found) { best = it->second; }
    }
    return best;
}

// ============================================================================
// RecallCoordTable
// ============================================================================
RecallCoordTable& RecallCoordTable::Get() { static RecallCoordTable s; return s; }
bool RecallCoordTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\RecallCoord.txt")) return false;
    if (const TsTable* t = f.Find("RecallPoint")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            RecallCoordRow x;
            x.kItemIndex  = t->GetStr(r, "ItemIndex");
            x.uiItemIdent = (uint32)t->GetInt(r, "ItemIdent");
            x.kMapName    = t->GetStr(r, "MapName");
            x.uiLinkX     = (uint16)t->GetInt(r, "LinkX");
            x.uiLinkY     = (uint16)t->GetInt(r, "LinkY");
            if (!x.kItemIndex.empty()) m_kRows[x.kItemIndex] = x;
        }
    }
    return true;
}
const RecallCoordRow* RecallCoordTable::FindByItem(const std::string& r) const {
    std::map<std::string, RecallCoordRow>::const_iterator it = m_kRows.find(r);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

// ============================================================================
// DamageByAngleTable / DamageBySoulTable
// ============================================================================
DamageByAngleTable& DamageByAngleTable::Get() { static DamageByAngleTable s; return s; }
bool DamageByAngleTable::Load(const std::string& rRoot) {
    m_kChr.clear(); m_kMob.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\DamageByAngle.txt")) return false;
    if (const TsTable* t = f.Find("DamageByAngle_Chr")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint16 a = (uint16)t->GetInt(r, "DamagedAngle");
            int32  s = (int32) t->GetInt(r, "DamageRate");
            m_kChr[a] = s;
        }
    }
    if (const TsTable* t = f.Find("DamageByAngle_Mob")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint16 a = (uint16)t->GetInt(r, "DamagedAngle");
            int32  s = (int32) t->GetInt(r, "DamageRate");
            m_kMob[a] = s;
        }
    }
    return true;
}
static int32 LookupAngle(const std::map<uint16, int32>& m, uint16 a) {
    if (m.empty()) return 1000;
    std::map<uint16, int32>::const_iterator it = m.lower_bound(a);
    if (it == m.end()) { --it; return it->second; }
    if (it == m.begin()) return it->second;
    int32 hi = it->second; uint16 hiA = it->first;
    --it;
    int32 lo = it->second; uint16 loA = it->first;
    if (hiA == loA) return lo;
    // Linear interpolate.
    return lo + (int32)((int64)(hi - lo) * (int64)(a - loA) / (int64)(hiA - loA));
}
int32 DamageByAngleTable::ChrAttackerScalerX1k(uint16 a) const { return LookupAngle(m_kChr, a); }
int32 DamageByAngleTable::MobAttackerScalerX1k(uint16 a) const { return LookupAngle(m_kMob, a); }

DamageBySoulTable& DamageBySoulTable::Get() { static DamageBySoulTable s; return s; }
bool DamageBySoulTable::Load(const std::string& rRoot) {
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) m_aMatrix[i][j] = 1000;
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\DamageBySoul.txt")) return false;
    const TsTable* t = f.Find("DamageBySoul"); if (!t) return false;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        int d = (int)t->GetInt(r, "DemandSoul");
        if (d < 0 || d >= 8) continue;
        char buf[12];
        for (int k = 0; k < 8; ++k) {
            sprintf_s(buf, sizeof(buf), "Soul%02d", k);
            int32 v = (int32)t->GetInt(r, buf, 1000);
            m_aMatrix[d][k] = v;
        }
    }
    return true;
}
int32 DamageBySoulTable::BonusX1k(uint8 dem, uint8 cnt) const {
    if (dem >= 8 || cnt >= 8) return 1000;
    return m_aMatrix[dem][cnt];
}

// ============================================================================
// ItemUseFunctionTable
// ============================================================================
ItemUseFunctionTable& ItemUseFunctionTable::Get() { static ItemUseFunctionTable s; return s; }
bool ItemUseFunctionTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\ItemUseFunction.txt")) return false;
    const TsTable* t = f.Find("ItemUseFunction"); if (!t) return false;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        ItemUseFunctionRow x;
        x.kItemIndex   = t->GetStr(r, "ItemIndex");
        x.uiBroadcast  = (uint8)t->GetInt(r, "BroadCast");
        x.kUseFunction = t->GetStr(r, "UseFunction");
        if (!x.kItemIndex.empty()) m_kRows[x.kItemIndex] = x;
    }
    return true;
}
const ItemUseFunctionRow* ItemUseFunctionTable::Find(const std::string& r) const {
    std::map<std::string, ItemUseFunctionRow>::const_iterator it = m_kRows.find(r);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

// ============================================================================
// RandomOptionTable / ItemDropGroupTable
// ============================================================================
RandomOptionTable& RandomOptionTable::Get() { static RandomOptionTable s; return s; }
bool RandomOptionTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\RandomOptionTable.txt")) return false;
    const TsTable* t = f.Find("RandomOptionTable"); if (!t) return false;
    static const char* kMin[5] = {"StrMin","ConMin","DexMin","IntMin","MenMin"};
    static const char* kMax[5] = {"StrMax","ConMax","DexMax","IntMax","MenMax"};
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        RandomOptionRow x;
        x.kDropItemIndex = t->GetStr(r, "DropItemIndex");
        x.uiOptionHide   = (uint8)t->GetInt(r, "OptionHide");
        x.uiMinOpCount   = (uint8)t->GetInt(r, "MinOpCount");
        x.uiMaxOpCount   = (uint8)t->GetInt(r, "MaxOpCount");
        for (int k = 0; k < 5; ++k) {
            x.aMin[k] = (uint16)t->GetInt(r, kMin[k]);
            x.aMax[k] = (uint16)t->GetInt(r, kMax[k]);
        }
        if (!x.kDropItemIndex.empty()) m_kRows[x.kDropItemIndex] = x;
    }
    return true;
}
const RandomOptionRow* RandomOptionTable::Find(const std::string& r) const {
    std::map<std::string, RandomOptionRow>::const_iterator it = m_kRows.find(r);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

ItemDropGroupTable& ItemDropGroupTable::Get() { static ItemDropGroupTable s; return s; }
bool ItemDropGroupTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\ItemDropGroup.txt")) return false;
    const TsTable* t = f.Find("ItemDropGroup"); if (!t) return false;
    char buf[16];
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        ItemDropGroupRow x;
        x.kItemGroupIdx = t->GetStr(r, "ItemGroupIdx");
        x.kItemID       = t->GetStr(r, "ItemID");
        x.uiMinQty      = (uint16)t->GetInt(r, "MinQtty");
        x.uiMaxQty      = (uint16)t->GetInt(r, "MaxQtty");
        for (int k = 0; k < 16; ++k) {
            sprintf_s(buf, sizeof(buf), "Upgrade%02d", k);
            x.aUpgradeChance[k] = (uint16)t->GetInt(r, buf);
        }
        if (!x.kItemGroupIdx.empty()) m_kRows[x.kItemGroupIdx] = x;
    }
    return true;
}
const ItemDropGroupRow* ItemDropGroupTable::Find(const std::string& r) const {
    std::map<std::string, ItemDropGroupRow>::const_iterator it = m_kRows.find(r);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

// ============================================================================
// PineScriptTable / SubLayerInteractTable / NPCActionTable
// ============================================================================
PineScriptTable& PineScriptTable::Get() { static PineScriptTable s; return s; }
bool PineScriptTable::Load(const std::string& rRoot) {
    m_kScripts.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\PineScript.txt")) return false;
    const TsTable* t = f.Find("PineScript"); if (!t) return false;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        std::string s = t->GetStr(r, "ScriptName");
        if (!s.empty()) m_kScripts.push_back(s);
    }
    return true;
}

// Load every ScenarioBookShelf script referenced by PineScript.txt, plus
// the always-on Wedding / Guild ceremonies. Each entry is "Cat/Name" or
// "Cat/Name/Name" (some scenarios live in their own subfolder); the loader
// splits on '/' and feeds (category, name) into ScenarioBookShelf.
size_t LoadAllPineScripts(const std::string& rRoot) {
    PineScriptTable::Get().Load(rRoot);
    const std::vector<std::string>& list = PineScriptTable::Get().Scripts();
    size_t loaded = 0;
    for (size_t i = 0; i < list.size(); ++i) {
        const std::string& s = list[i];
        size_t slash = s.find('/');
        if (slash == std::string::npos) continue;
        std::string cat  = s.substr(0, slash);
        std::string tail = s.substr(slash + 1);
        // If the tail has another '/', the leaf is the deepest segment.
        size_t s2 = tail.rfind('/');
        std::string name = (s2 == std::string::npos) ? tail : tail.substr(s2 + 1);
        std::string subcat = (s2 == std::string::npos) ? cat : (cat + "\\" + tail.substr(0, s2));
        if (ScenarioBookShelf::Get().Load(rRoot, subcat, name)) ++loaded;
    }
    // Always-on ceremonies that the data file may omit.
    if (ScenarioBookShelf::Get().Load(rRoot, "Wedding", "Wedding"))                ++loaded;
    if (ScenarioBookShelf::Get().Load(rRoot, "Guild",   "GuildTournament"))        ++loaded;
    if (ScenarioBookShelf::Get().Load(rRoot, "Guild",   "GuildTournament1"))       ++loaded;
    return loaded;
}

SubLayerInteractTable& SubLayerInteractTable::Get() { static SubLayerInteractTable s; return s; }
bool SubLayerInteractTable::Load(const std::string& rRoot) {
    memset(m_aBrief, 0, sizeof(m_aBrief));
    memset(m_aAttack, 0, sizeof(m_aAttack));
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\SubLayerInteract.txt")) return false;
    static const char* kCols[6] = {
        "Base","RangerStealth","Observer","AdminHide","GMDoor","GMPlayer"
    };
    if (const TsTable* t = f.Find("SendMyBrief")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint8 my = (uint8)t->GetInt(r, "CheckSumA");
            if (my >= 8) continue;
            for (uint8 c = 0; c < 6; ++c)
                m_aBrief[my][c] = (uint8)t->GetInt(r, kCols[c]);
        }
    }
    if (const TsTable* t = f.Find("CanAttack")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint8 my = (uint8)t->GetInt(r, "CheckSumA");
            if (my >= 8) continue;
            for (uint8 c = 0; c < 6; ++c)
                m_aAttack[my][c] = (uint8)t->GetInt(r, kCols[c]);
        }
    }
    return true;
}
bool SubLayerInteractTable::CanSeeMyBrief(uint8 m, uint8 o) const {
    return (m < 8 && o < 8) ? (m_aBrief[m][o] != 0) : false;
}
bool SubLayerInteractTable::CanAttack(uint8 m, uint8 o) const {
    return (m < 8 && o < 8) ? (m_aAttack[m][o] != 0) : false;
}

NPCActionTable& NPCActionTable::Get() { static NPCActionTable s; return s; }
bool NPCActionTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\NPCAction.txt")) return false;
    const TsTable* t = f.Find("NPCCondition"); if (!t) return false;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        Row x;
        x.uiConditionID = (uint8)t->GetInt(r, "ConditionID");
        x.kConditionA   = t->GetStr(r, "ConditionA");
        x.kTypeA        = t->GetStr(r, "TypeA");
        x.uiAX          = (uint32)t->GetInt(r, "AX");
        x.uiAY          = (uint32)t->GetInt(r, "AY");
        x.kConditionB   = t->GetStr(r, "ConditionB");
        x.kTypeB        = t->GetStr(r, "TypeB");
        x.uiBX          = (uint32)t->GetInt(r, "BX");
        x.uiBY          = (uint32)t->GetInt(r, "BY");
        m_kRows.push_back(x);
    }
    return true;
}

// =============================================================================
// KarenAttackTable -- World/Karen.txt
// =============================================================================
KarenAttackTable& KarenAttackTable::Get() { static KarenAttackTable s; return s; }
bool KarenAttackTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\Karen.txt")) return false;
    const TsTable* t = f.Find("AttSeq"); if (!t) return false;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        uint16 ord = (uint16)t->GetInt(r, "Order");
        std::string atk = t->GetStr(r, "Attack");
        if (atk == "-") atk.clear();
        m_kRows[ord] = atk;
    }
    SHINELOG_INFO("Karen.txt: %u attack steps loaded", (uint32)m_kRows.size());
    return true;
}
const std::string& KarenAttackTable::AttackAt(uint16 uiOrder) const {
    std::map<uint16, std::string>::const_iterator it = m_kRows.find(uiOrder);
    return (it == m_kRows.end()) ? m_kEmpty : it->second;
}

// =============================================================================
// MobChatTable -- World/MobChat.txt (six tables; same column shape).
// =============================================================================
MobChatTable& MobChatTable::Get() { static MobChatTable s; return s; }
bool MobChatTable::Load(const std::string& rRoot) {
    for (int b = 0; b < MC_BUCKETS; ++b) { m_kBucket[b].clear(); m_kIndex[b].clear(); }
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\MobChat.txt")) return false;
    static const char* kNames[MC_BUCKETS] = {
        "PIECE", "ATTACK", "DAMAGED", "DEAD", "HELPMAIN", "HELPSUB"
    };
    for (int b = 0; b < MC_BUCKETS; ++b) {
        const TsTable* t = f.Find(kNames[b]); if (!t) continue;
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            Row x;
            x.kMobIndex = t->GetStr(r, "MobIndex");
            x.uiRate0   = (uint32)t->GetInt(r, "Rate0");
            x.uiRate1   = (uint32)t->GetInt(r, "Rate1");
            x.aScript[0]= t->GetStr(r, "Script0");
            x.aScript[1]= t->GetStr(r, "Script1");
            x.aScript[2]= t->GetStr(r, "Script2");
            x.aScript[3]= t->GetStr(r, "Script3");
            m_kIndex[b][x.kMobIndex].push_back(m_kBucket[b].size());
            m_kBucket[b].push_back(x);
        }
    }
    return true;
}
std::string MobChatTable::Pick(eMobChatBucket eB, const std::string& rM) const {
    if ((int)eB < 0 || (int)eB >= MC_BUCKETS) return std::string();
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kIndex[eB].find(rM);
    if (it == m_kIndex[eB].end()) return std::string();
    if (it->second.empty()) return std::string();
    const Row& r = m_kBucket[eB][it->second[0]];
    uint32 dice = (uint32)(rand() % 100);
    if (dice < r.uiRate0)                         return r.aScript[0];
    if (dice < r.uiRate0 + r.uiRate1)             return r.aScript[1];
    // Remaining 0..3 fan out by quartile across (Script2, Script3).
    if (dice < r.uiRate0 + r.uiRate1 + 50)        return r.aScript[2];
    return r.aScript[3];
}

} // namespace fiesta
