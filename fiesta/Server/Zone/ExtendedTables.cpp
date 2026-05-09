// Server/Zone/ExtendedTables.cpp
// Each Bind() walks one ShnFile and projects a small set of named
// columns into a typed POD vector + index map. Where a column is
// absent in this drop, ShnGetU32/Str/I32 returns 0 / "" / 0 and the
// row is recorded with that fallback -- the runtime path handles the
// "no row" case via the Find() returning NULL.
#include "ExtendedTables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

namespace {
inline const ShnFile* T(const char* szName) {
    return ShnRegistry::Get().GetTable(szName);
}
} // namespace

#define ITER(VAR) for (size_t _r = 0; _r < (VAR)->Rows().size(); ++_r)

// -----------------------------------------------------------------------------
//  AccUpgradeTables
// -----------------------------------------------------------------------------
AccUpgradeTables& AccUpgradeTables::Get() { static AccUpgradeTables s; return s; }
void AccUpgradeTables::Bind() {
    if (const ShnFile* t = T("AccUpgrade")) ITER(t) {
        AccRow r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.uiCriFail    = ShnGetU32(*t, _r, "CriFail");
        r.uiDownFail   = ShnGetU32(*t, _r, "DownFail");
        r.uiNormalFail = ShnGetU32(*t, _r, "NormalFail");
        r.uiCon        = ShnGetU32(*t, _r, "Con");
        r.uiLuckySuc   = ShnGetU32(*t, _r, "LuckySuc");
        m_kAccById[r.uiID] = m_kAcc.size();
        m_kAcc.push_back(r);
    }
    if (const ShnFile* t = T("AccUpGradeInfo")) ITER(t) {
        InfoRow r;
        r.uiID       = ShnGetU32(*t, _r, "ID");
        r.kInxName   = ShnGetStr(*t, _r, "InxName");
        r.uiUpFactor = ShnGetU32(*t, _r, "UpFactor");
        r.uiSuccess  = ShnGetU32(*t, _r, "Success");
        r.uiMin1     = ShnGetU32(*t, _r, "Min1");
        r.uiMax1     = ShnGetU32(*t, _r, "Max1");
        r.uiMin2     = ShnGetU32(*t, _r, "Min2");
        r.uiMax2     = ShnGetU32(*t, _r, "Max2");
        m_kInfoById[r.uiID] = m_kInfo.size();
        m_kInfo.push_back(r);
    }
    if (const ShnFile* t = T("BRAccUpgrade")) ITER(t) {
        AccRow r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.uiCriFail    = ShnGetU32(*t, _r, "CriFail");
        r.uiDownFail   = ShnGetU32(*t, _r, "DownFail");
        r.uiNormalFail = ShnGetU32(*t, _r, "NormalFail");
        r.uiCon        = ShnGetU32(*t, _r, "Con");
        r.uiLuckySuc   = ShnGetU32(*t, _r, "LuckySuc");
        m_kBRById[r.uiID] = m_kBR.size();
        m_kBR.push_back(r);
    }
    if (const ShnFile* t = T("BRAccUpgradeInfo")) ITER(t) {
        InfoRow r;
        r.uiID       = ShnGetU32(*t, _r, "ID");
        r.kInxName   = ShnGetStr(*t, _r, "InxName");
        r.uiUpFactor = ShnGetU32(*t, _r, "UpFactor");
        r.uiSuccess  = ShnGetU32(*t, _r, "Success");
        r.uiMin1     = ShnGetU32(*t, _r, "Min1");
        r.uiMax1     = ShnGetU32(*t, _r, "Max1");
        r.uiMin2     = ShnGetU32(*t, _r, "Min2");
        r.uiMax2     = ShnGetU32(*t, _r, "Max2");
        m_kBRInfoById[r.uiID] = m_kBRInfo.size();
        m_kBRInfo.push_back(r);
    }
}
const AccUpgradeTables::AccRow*  AccUpgradeTables::FindAcc (uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kAccById.find(u);
    return (it == m_kAccById.end()) ? NULL : &m_kAcc[it->second]; }
const AccUpgradeTables::InfoRow* AccUpgradeTables::FindInfo(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kInfoById.find(u);
    return (it == m_kInfoById.end()) ? NULL : &m_kInfo[it->second]; }
const AccUpgradeTables::AccRow*  AccUpgradeTables::FindBR(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kBRById.find(u);
    return (it == m_kBRById.end()) ? NULL : &m_kBR[it->second]; }
const AccUpgradeTables::InfoRow* AccUpgradeTables::FindBRInfo(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kBRInfoById.find(u);
    return (it == m_kBRInfoById.end()) ? NULL : &m_kBRInfo[it->second]; }

// -----------------------------------------------------------------------------
//  ActionExtraTables
// -----------------------------------------------------------------------------
ActionExtraTables& ActionExtraTables::Get() { static ActionExtraTables s; return s; }
void ActionExtraTables::Bind() {
    if (const ShnFile* t = T("ActionEffectAbState")) ITER(t) {
        AESRow r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.uiAbState    = ShnGetU32(*t, _r, "AbState");
        r.uiStrength   = ShnGetU32(*t, _r, "Strength");
        r.uiKeepTimeMs = ShnGetU32(*t, _r, "KeepTime");
        m_kAESById[r.uiID] = m_kAES.size();
        m_kAES.push_back(r);
    }
    if (const ShnFile* t = T("ActionViewInfo")) ITER(t) {
        ViewRow r;
        r.uiID     = ShnGetU32(*t, _r, "ID");
        r.kInxName = ShnGetStr(*t, _r, "InxName");
        r.uiAniNo  = ShnGetU32(*t, _r, "AniNo");
        m_kViewById[r.uiID] = m_kView.size();
        m_kView.push_back(r);
    }
    if (const ShnFile* t = T("ActiveSkillGroup")) ITER(t) {
        SkillGroupRow r;
        r.uiID       = ShnGetU32(*t, _r, "ID");
        r.kGroupName = ShnGetStr(*t, _r, "GroupName");
        r.kSkillInx  = ShnGetStr(*t, _r, "SkillInx");
        m_kByGroup[r.kGroupName].push_back(m_kGroup.size());
        m_kGroup.push_back(r);
    }
}
const ActionExtraTables::AESRow* ActionExtraTables::FindEffectAbState(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kAESById.find(u);
    return (it == m_kAESById.end()) ? NULL : &m_kAES[it->second]; }
const ActionExtraTables::ViewRow* ActionExtraTables::FindActionView(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kViewById.find(u);
    return (it == m_kViewById.end()) ? NULL : &m_kView[it->second]; }
void ActionExtraTables::SkillsByGroup(const std::string& rG,
                                      std::vector<const SkillGroupRow*>& rOut) const {
    rOut.clear();
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kByGroup.find(rG);
    if (it == m_kByGroup.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kGroup[it->second[i]]);
}

// -----------------------------------------------------------------------------
//  AdminLvSet / AnnounceData / BMP
// -----------------------------------------------------------------------------
AdminLvSetTable& AdminLvSetTable::Get() { static AdminLvSetTable s; return s; }
void AdminLvSetTable::Bind() {
    if (const ShnFile* t = T("AdminLvSet")) ITER(t)
        m_kRows[ShnGetU32(*t, _r, "ID")] = (uint8)ShnGetU32(*t, _r, "AdminLv");
}
uint8 AdminLvSetTable::RequiredFor(uint32 u) const {
    std::map<uint32,uint8>::const_iterator it = m_kRows.find(u);
    return (it == m_kRows.end()) ? 0 : it->second;
}

AnnounceDataTable& AnnounceDataTable::Get() { static AnnounceDataTable s; return s; }
void AnnounceDataTable::Bind() {
    if (const ShnFile* t = T("AnnounceData")) ITER(t) {
        Row r;
        r.uiHandle  = ShnGetU32(*t, _r, "Handle");
        r.kMessage  = ShnGetStr(*t, _r, "Message");
        r.uiColor   = ShnGetU32(*t, _r, "Color");
        m_kByHandle[r.uiHandle] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const AnnounceDataTable::Row* AnnounceDataTable::Find(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kByHandle.find(u);
    return (it == m_kByHandle.end()) ? NULL : &m_kRows[it->second]; }

BMPTable& BMPTable::Get() { static BMPTable s; return s; }
void BMPTable::Bind() {
    if (const ShnFile* t = T("BMP")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "BMPIndex") << 32) | ShnGetU32(*t, _r, "Step");
        m_kKeys[k] = true;
    }
}
bool BMPTable::Has(uint32 b, uint32 s) const {
    uint64 k = ((uint64)b << 32) | s;
    return m_kKeys.find(k) != m_kKeys.end();
}

// -----------------------------------------------------------------------------
//  BelongTables
// -----------------------------------------------------------------------------
BelongTables& BelongTables::Get() { static BelongTables s; return s; }
void BelongTables::Bind() {
    if (const ShnFile* t = T("BelongDice")) ITER(t) {
        DiceRow r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.uiBelongType = ShnGetU32(*t, _r, "BelongType");
        r.uiRate       = ShnGetU32(*t, _r, "Rate");
        m_kDiceById[r.uiID] = m_kDice.size();
        m_kDice.push_back(r);
    }
    if (const ShnFile* t = T("BelongTypeInfo")) ITER(t) {
        TypeInfoRow r;
        r.uiBelongType = ShnGetU32(*t, _r, "BelongType");
        r.uiCanTrade   = ShnGetU32(*t, _r, "CanTrade");
        r.uiCanDrop    = ShnGetU32(*t, _r, "CanDrop");
        r.uiCanStorage = ShnGetU32(*t, _r, "CanStorage");
        m_kTypeById[r.uiBelongType] = m_kType.size();
        m_kType.push_back(r);
    }
}
const BelongTables::DiceRow* BelongTables::FindDice(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kDiceById.find(u);
    return (it == m_kDiceById.end()) ? NULL : &m_kDice[it->second]; }
const BelongTables::TypeInfoRow* BelongTables::FindType(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kTypeById.find(u);
    return (it == m_kTypeById.end()) ? NULL : &m_kType[it->second]; }

// -----------------------------------------------------------------------------
//  CharTitleStateServer / ChatColor / Hair+Color
// -----------------------------------------------------------------------------
CharTitleStateServerTable& CharTitleStateServerTable::Get() { static CharTitleStateServerTable s; return s; }
void CharTitleStateServerTable::Bind() {
    if (const ShnFile* t = T("CharacterTitleStateServer")) ITER(t) {
        Row r;
        r.uiTitleID = ShnGetU32(*t, _r, "TitleID");
        r.iStr      = ShnGetI32(*t, _r, "Str");
        r.iCon      = ShnGetI32(*t, _r, "Con");
        r.iDex      = ShnGetI32(*t, _r, "Dex");
        r.iInt      = ShnGetI32(*t, _r, "Int");
        r.iMen      = ShnGetI32(*t, _r, "Men");
        r.uiAtkSpd  = ShnGetU32(*t, _r, "AtkSpd");
        r.uiCastSpd = ShnGetU32(*t, _r, "CastSpd");
        r.uiCoolDown= ShnGetU32(*t, _r, "CoolDown");
        m_kById[r.uiTitleID] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const CharTitleStateServerTable::Row* CharTitleStateServerTable::Find(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kById.find(u);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second]; }

ChatColorTable& ChatColorTable::Get() { static ChatColorTable s; return s; }
void ChatColorTable::Bind() {
    if (const ShnFile* t = T("ChatColor")) ITER(t)
        m_kColor[ShnGetU32(*t, _r, "ChannelID")] = ShnGetU32(*t, _r, "Color");
}
uint32 ChatColorTable::Color(uint32 u) const {
    std::map<uint32,uint32>::const_iterator it = m_kColor.find(u);
    return (it == m_kColor.end()) ? 0 : it->second;
}

ColorInfoTables& ColorInfoTables::Get() { static ColorInfoTables s; return s; }
void ColorInfoTables::Bind() {
    if (const ShnFile* t = T("ColorInfo")) ITER(t) {
        ColorRow r;
        r.uiID    = ShnGetU32(*t, _r, "ID");
        r.kName   = ShnGetStr(*t, _r, "Name");
        r.uiRGBA  = ShnGetU32(*t, _r, "RGBA");
        m_kColorById[r.uiID] = m_kColor.size();
        m_kColor.push_back(r);
    }
    if (const ShnFile* t = T("HairColorInfo")) ITER(t) {
        HairColorRow r;
        r.uiID    = ShnGetU32(*t, _r, "ID");
        r.kName   = ShnGetStr(*t, _r, "Name");
        r.uiRGBA  = ShnGetU32(*t, _r, "RGBA");
        r.uiGrade = ShnGetU32(*t, _r, "Grade");
        m_kHairById[r.uiID] = m_kHair.size();
        m_kHair.push_back(r);
    }
}
const ColorInfoTables::ColorRow* ColorInfoTables::FindColor(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kColorById.find(u);
    return (it == m_kColorById.end()) ? NULL : &m_kColor[it->second]; }
const ColorInfoTables::HairColorRow* ColorInfoTables::FindHair(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kHairById.find(u);
    return (it == m_kHairById.end()) ? NULL : &m_kHair[it->second]; }

// -----------------------------------------------------------------------------
//  CollectExtras
// -----------------------------------------------------------------------------
CollectExtrasTables& CollectExtrasTables::Get() { static CollectExtrasTables s; return s; }
void CollectExtrasTables::Bind() {
    if (const ShnFile* t = T("CollectCardDropRate")) ITER(t)
        m_kDrop[ShnGetU32(*t, _r, "CardID")] = ShnGetU32(*t, _r, "Percent");
    if (const ShnFile* t = T("CollectCardGroupDesc")) ITER(t)
        m_kGroupDesc[ShnGetU32(*t, _r, "MobGroup")] = ShnGetStr(*t, _r, "Description");
    if (const ShnFile* t = T("CollectCardMobGroup")) ITER(t)
        m_kGroupMobs[ShnGetU32(*t, _r, "MobGroup")].push_back(ShnGetStr(*t, _r, "MobInx"));
    if (const ShnFile* t = T("CollectCardStarRate")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "CardGradeType") << 32) | ShnGetU32(*t, _r, "Star");
        m_kStar[k] = ShnGetU32(*t, _r, "Percent");
    }
    if (const ShnFile* t = T("CollectCardTitle")) ITER(t)
        m_kTitle[ShnGetU32(*t, _r, "CardSetID")] = ShnGetU32(*t, _r, "TitleID");
}
int32 CollectExtrasTables::DropPercent(uint32 u) const {
    std::map<uint32,uint32>::const_iterator it = m_kDrop.find(u);
    return (it == m_kDrop.end()) ? 0 : (int32)it->second;
}
const std::string& CollectExtrasTables::GroupDesc(uint32 u) const {
    std::map<uint32,std::string>::const_iterator it = m_kGroupDesc.find(u);
    return (it == m_kGroupDesc.end()) ? m_kEmpty : it->second;
}
void CollectExtrasTables::MobsInGroup(uint32 u, std::vector<std::string>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<std::string> >::const_iterator it = m_kGroupMobs.find(u);
    if (it != m_kGroupMobs.end()) rOut = it->second;
}
int32 CollectExtrasTables::StarPercent(uint32 g, uint32 s) const {
    uint64 k = ((uint64)g << 32) | s;
    std::map<uint64,uint32>::const_iterator it = m_kStar.find(k);
    return (it == m_kStar.end()) ? 0 : (int32)it->second;
}
int32 CollectExtrasTables::TitleForSet(uint32 u) const {
    std::map<uint32,uint32>::const_iterator it = m_kTitle.find(u);
    return (it == m_kTitle.end()) ? 0 : (int32)it->second;
}

// -----------------------------------------------------------------------------
//  DiceTables
// -----------------------------------------------------------------------------
DiceTables& DiceTables::Get() { static DiceTables s; return s; }
void DiceTables::Bind() {
    if (const ShnFile* t = T("DiceGame")) ITER(t) {
        GameRow r;
        r.uiID    = ShnGetU32(*t, _r, "ID");
        r.kName   = ShnGetStr(*t, _r, "Name");
        r.uiBet   = ShnGetU32(*t, _r, "Bet");
        r.uiTimeMs= ShnGetU32(*t, _r, "Time");
        m_kGameById[r.uiID] = m_kGame.size();
        m_kGame.push_back(r);
    }
    if (const ShnFile* t = T("DiceRate")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "GameID") << 32) | ShnGetU32(*t, _r, "Outcome");
        m_kRate[k] = ShnGetU32(*t, _r, "Rate");
    }
    if (const ShnFile* t = T("DiceDividind")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "GameID") << 32) | ShnGetU32(*t, _r, "Tier");
        m_kDiv[k] = ShnGetU32(*t, _r, "Dividend");
    }
}
const DiceTables::GameRow* DiceTables::FindGame(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kGameById.find(u);
    return (it == m_kGameById.end()) ? NULL : &m_kGame[it->second]; }
int32 DiceTables::Rate(uint32 g, uint32 o) const {
    uint64 k = ((uint64)g << 32) | o;
    std::map<uint64,uint32>::const_iterator it = m_kRate.find(k);
    return (it == m_kRate.end()) ? 0 : (int32)it->second;
}
int32 DiceTables::Dividend(uint32 g, uint32 ti) const {
    uint64 k = ((uint64)g << 32) | ti;
    std::map<uint64,uint32>::const_iterator it = m_kDiv.find(k);
    return (it == m_kDiv.end()) ? 0 : (int32)it->second;
}

// -----------------------------------------------------------------------------
//  EnchantSocketTable / ErrorCodeTable
// -----------------------------------------------------------------------------
EnchantSocketTable& EnchantSocketTable::Get() { static EnchantSocketTable s; return s; }
void EnchantSocketTable::Bind() {
    if (const ShnFile* t = T("EnchantSocketRate")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "Grade") << 32) | ShnGetU32(*t, _r, "SocketSlot");
        m_kRate[k] = ShnGetU32(*t, _r, "Rate");
    }
}
int32 EnchantSocketTable::RatePerThousand(uint32 g, uint32 s) const {
    uint64 k = ((uint64)g << 32) | s;
    std::map<uint64,uint32>::const_iterator it = m_kRate.find(k);
    return (it == m_kRate.end()) ? 0 : (int32)it->second;
}

ErrorCodeTable& ErrorCodeTable::Get() { static ErrorCodeTable s; return s; }
void ErrorCodeTable::Bind() {
    if (const ShnFile* t = T("ErrorCodeTable")) ITER(t) {
        uint32 c = ShnGetU32(*t, _r, "Code");
        std::string k = ShnGetStr(*t, _r, "Key");
        std::string m = ShnGetStr(*t, _r, "Message");
        m_kByCode[c] = m;
        m_kByKey[k]  = c;
    }
}
const std::string& ErrorCodeTable::Message(uint32 u) const {
    std::map<uint32,std::string>::const_iterator it = m_kByCode.find(u);
    return (it == m_kByCode.end()) ? m_kEmpty : it->second;
}
uint32 ErrorCodeTable::CodeOf(const std::string& rK) const {
    std::map<std::string,uint32>::const_iterator it = m_kByKey.find(rK);
    return (it == m_kByKey.end()) ? 0 : it->second;
}

// -----------------------------------------------------------------------------
//  EstateExtraTables
// -----------------------------------------------------------------------------
EstateExtraTables& EstateExtraTables::Get() { static EstateExtraTables s; return s; }
void EstateExtraTables::Bind() {
    if (const ShnFile* t = T("MiniHouseDummy")) ITER(t) {
        DummyRow r;
        r.uiHouseID = ShnGetU32(*t, _r, "HouseID");
        r.uiSlotID  = ShnGetU32(*t, _r, "SlotID");
        r.kBoneName = ShnGetStr(*t, _r, "BoneName");
        m_kDummyByHouse[r.uiHouseID].push_back(m_kDummy.size());
        m_kDummy.push_back(r);
    }
    if (const ShnFile* t = T("MiniHouseEndure")) ITER(t) {
        EndureRow r;
        r.uiTier        = ShnGetU32(*t, _r, "Tier");
        r.uiMaxEndure   = ShnGetU32(*t, _r, "MaxEndure");
        r.uiHourlyDecay = ShnGetU32(*t, _r, "HourlyDecay");
        m_kEndureByTier[r.uiTier] = m_kEndure.size();
        m_kEndure.push_back(r);
    }
    if (const ShnFile* t = T("MiniHouseFurnitureObjEffect")) ITER(t) {
        FurnEffRow r;
        r.uiFurnID    = ShnGetU32(*t, _r, "FurnID");
        r.uiAbStateID = ShnGetU32(*t, _r, "AbStateID");
        r.uiRange     = ShnGetU32(*t, _r, "Range");
        m_kFurnEffById[r.uiFurnID] = m_kFurnEff.size();
        m_kFurnEff.push_back(r);
    }
    if (const ShnFile* t = T("MiniHouseObjAni")) ITER(t) {
        ObjAniRow r;
        r.uiFurnID  = ShnGetU32(*t, _r, "FurnID");
        r.uiAnimSlot= ShnGetU32(*t, _r, "AnimSlot");
        r.kAniName  = ShnGetStr(*t, _r, "AniName");
        r.uiLoopMs  = ShnGetU32(*t, _r, "LoopMs");
        uint64 k = ((uint64)r.uiFurnID << 32) | r.uiAnimSlot;
        m_kObjAniByKey[k] = m_kObjAni.size();
        m_kObjAni.push_back(r);
    }
}
void EstateExtraTables::DummiesFor(uint32 u, std::vector<const DummyRow*>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<size_t> >::const_iterator it = m_kDummyByHouse.find(u);
    if (it == m_kDummyByHouse.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kDummy[it->second[i]]);
}
const EstateExtraTables::EndureRow* EstateExtraTables::FindEndure(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kEndureByTier.find(u);
    return (it == m_kEndureByTier.end()) ? NULL : &m_kEndure[it->second]; }
const EstateExtraTables::FurnEffRow* EstateExtraTables::FindFurnEff(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kFurnEffById.find(u);
    return (it == m_kFurnEffById.end()) ? NULL : &m_kFurnEff[it->second]; }
const EstateExtraTables::ObjAniRow* EstateExtraTables::FindObjAni(uint32 f, uint32 s) const {
    uint64 k = ((uint64)f << 32) | s;
    std::map<uint64,size_t>::const_iterator it = m_kObjAniByKey.find(k);
    return (it == m_kObjAniByKey.end()) ? NULL : &m_kObjAni[it->second]; }

// -----------------------------------------------------------------------------
//  FieldLvCondition / FriendPointReward / SetItemEffect
// -----------------------------------------------------------------------------
FieldLvConditionTable& FieldLvConditionTable::Get() { static FieldLvConditionTable s; return s; }
void FieldLvConditionTable::Bind() {
    if (const ShnFile* t = T("FieldLvCondition")) ITER(t) {
        Row r;
        r.uiMin = ShnGetU32(*t, _r, "MinLv");
        r.uiMax = ShnGetU32(*t, _r, "MaxLv");
        m_kRow[ShnGetStr(*t, _r, "MapName")] = r;
    }
}
bool FieldLvConditionTable::IsAllowed(const std::string& rM, uint32 uL) const {
    std::map<std::string, Row>::const_iterator it = m_kRow.find(rM);
    if (it == m_kRow.end()) return true;     // no rule -> open
    return uL >= it->second.uiMin && (it->second.uiMax == 0 || uL <= it->second.uiMax);
}

FriendPointRewardTable& FriendPointRewardTable::Get() { static FriendPointRewardTable s; return s; }
void FriendPointRewardTable::Bind() {
    if (const ShnFile* t = T("FriendPointReward")) ITER(t) {
        Row r;
        r.uiThreshold = ShnGetU32(*t, _r, "Threshold");
        r.kRewardItem = ShnGetStr(*t, _r, "RewardItem");
        r.uiQty       = ShnGetU32(*t, _r, "Qty");
        m_kByThreshold[r.uiThreshold] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const FriendPointRewardTable::Row* FriendPointRewardTable::Find(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kByThreshold.find(u);
    return (it == m_kByThreshold.end()) ? NULL : &m_kRows[it->second]; }

SetItemEffectTable& SetItemEffectTable::Get() { static SetItemEffectTable s; return s; }
void SetItemEffectTable::Bind() {
    if (const ShnFile* t = T("SetItemEffect")) ITER(t) {
        Row r;
        r.uiEffectID  = ShnGetU32(*t, _r, "EffectID");
        r.kEffectName = ShnGetStr(*t, _r, "EffectName");
        r.iValue      = ShnGetI32(*t, _r, "Value");
        m_kById[r.uiEffectID] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const SetItemEffectTable::Row* SetItemEffectTable::Find(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kById.find(u);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second]; }

// -----------------------------------------------------------------------------
//  GroupAbState
// -----------------------------------------------------------------------------
GroupAbStateTable& GroupAbStateTable::Get() { static GroupAbStateTable s; return s; }
void GroupAbStateTable::Bind() {
    if (const ShnFile* t = T("GroupAbState")) ITER(t) {
        Row r;
        r.uiSourceAbState     = ShnGetU32(*t, _r, "SourceAbState");
        r.uiPropagatedAbState = ShnGetU32(*t, _r, "PropagatedAbState");
        r.uiRange             = ShnGetU32(*t, _r, "Range");
        m_kBySrc[r.uiSourceAbState] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const GroupAbStateTable::Row* GroupAbStateTable::Find(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kBySrc.find(u);
    return (it == m_kBySrc.end()) ? NULL : &m_kRows[it->second]; }

// -----------------------------------------------------------------------------
//  GBTables (Guild Battle casino + house + tax + reward)
// -----------------------------------------------------------------------------
GBTables& GBTables::Get() { static GBTables s; return s; }
void GBTables::Bind() {
    if (const ShnFile* t = T("GBBanTime")) ITER(t)
        m_kBan[ShnGetU32(*t, _r, "HourOfWeek")] = true;
    if (const ShnFile* t = T("GBExchangeMaxCoin")) ITER(t)
        m_kExchange[ShnGetU32(*t, _r, "Tier")] = ShnGetI32(*t, _r, "MaxCoin");
    if (const ShnFile* t = T("GBHouse")) ITER(t)
        m_kHouse[ShnGetU32(*t, _r, "Tier")] = ShnGetI32(*t, _r, "Rent");
    if (const ShnFile* t = T("GBTaxRate")) ITER(t)
        m_kTax[ShnGetU32(*t, _r, "Tier")] = ShnGetI32(*t, _r, "Rate");
    if (const ShnFile* t = T("GBJoinGameMember")) ITER(t)
        m_kJoin[ShnGetU32(*t, _r, "CharLv")] = true;
    if (const ShnFile* t = T("GBDiceGame")) ITER(t) {
        DiceGame g;
        g.uiID    = ShnGetU32(*t, _r, "ID");
        g.kName   = ShnGetStr(*t, _r, "Name");
        g.uiMinBet= ShnGetU32(*t, _r, "MinBet");
        g.uiMaxBet= ShnGetU32(*t, _r, "MaxBet");
        m_kDiceGameById[g.uiID] = m_kDiceGame.size();
        m_kDiceGame.push_back(g);
    }
    if (const ShnFile* t = T("GBDiceRate")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "GameID") << 32) | ShnGetU32(*t, _r, "Outcome");
        m_kDiceRate[k] = ShnGetU32(*t, _r, "Rate");
    }
    if (const ShnFile* t = T("GBDiceDividind")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "GameID") << 32) | ShnGetU32(*t, _r, "Tier");
        m_kDiceDiv[k] = ShnGetU32(*t, _r, "Dividend");
    }
    if (const ShnFile* t = T("GBSMLine")) ITER(t) {
        SMRow r;
        r.uiID       = ShnGetU32(*t, _r, "ID");
        r.uiBetCoin  = ShnGetU32(*t, _r, "BetCoin");
        r.uiCardRate = ShnGetU32(*t, _r, "CardRate");
        r.uiJPRate   = ShnGetU32(*t, _r, "JPRate");
        r.uiLine     = ShnGetU32(*t, _r, "Line");
        m_kSMById[r.uiID] = m_kSM.size();
        m_kSM.push_back(r);
    }
    if (const ShnFile* t = T("GBSMBetCoin")) ITER(t)
        m_kSMAll[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "BetCoin");
    if (const ShnFile* t = T("GBSMCardRate")) ITER(t)
        m_kSMCenter[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "CardRate");
    if (const ShnFile* t = T("GBSMCenter")) ITER(t)
        m_kSMCenter[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "Center");
    if (const ShnFile* t = T("GBSMGroup")) ITER(t)
        m_kSMGroup[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "Group");
    if (const ShnFile* t = T("GBSMJPRate")) ITER(t)
        m_kSMAll[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "JPRate");
    if (const ShnFile* t = T("GBSMNPC")) ITER(t)
        m_kSMNPC[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "NPC");
    if (const ShnFile* t = T("GBSMAll")) ITER(t)
        m_kSMAll[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "All");
    if (const ShnFile* t = T("GBEventCode")) ITER(t)
        m_kEvent[ShnGetStr(*t, _r, "Event")] = ShnGetI32(*t, _r, "Code");
    if (const ShnFile* t = T("GBReward")) ITER(t) {
        RewardRow rr;
        rr.kItem = ShnGetStr(*t, _r, "RewardItem");
        rr.uiQty = ShnGetU32(*t, _r, "Qty");
        m_kReward[ShnGetU32(*t, _r, "Handle")] = rr;
    }
}
bool GBTables::IsBanned(uint32 u) const { return m_kBan.find(u) != m_kBan.end(); }
int32 GBTables::ExchangeMaxCoin(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kExchange.find(u);
    return (it == m_kExchange.end()) ? 0 : it->second; }
int32 GBTables::HouseRent(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kHouse.find(u);
    return (it == m_kHouse.end()) ? 0 : it->second; }
int32 GBTables::TaxRate(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kTax.find(u);
    return (it == m_kTax.end()) ? 0 : it->second; }
bool GBTables::CanJoin(uint32 u) const { return m_kJoin.find(u) != m_kJoin.end(); }
const GBTables::DiceGame* GBTables::FindDice(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kDiceGameById.find(u);
    return (it == m_kDiceGameById.end()) ? NULL : &m_kDiceGame[it->second]; }
int32 GBTables::DiceRate(uint32 g, uint32 o) const {
    uint64 k = ((uint64)g << 32) | o;
    std::map<uint64,uint32>::const_iterator it = m_kDiceRate.find(k);
    return (it == m_kDiceRate.end()) ? 0 : (int32)it->second; }
int32 GBTables::DiceDividend(uint32 g, uint32 ti) const {
    uint64 k = ((uint64)g << 32) | ti;
    std::map<uint64,uint32>::const_iterator it = m_kDiceDiv.find(k);
    return (it == m_kDiceDiv.end()) ? 0 : (int32)it->second; }
const GBTables::SMRow* GBTables::FindSM(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kSMById.find(u);
    return (it == m_kSMById.end()) ? NULL : &m_kSM[it->second]; }
int32 GBTables::SMAll   (uint32 u) const { std::map<uint32,int32>::const_iterator it = m_kSMAll.find(u);    return (it == m_kSMAll.end())    ? 0 : it->second; }
int32 GBTables::SMCenter(uint32 u) const { std::map<uint32,int32>::const_iterator it = m_kSMCenter.find(u); return (it == m_kSMCenter.end()) ? 0 : it->second; }
int32 GBTables::SMGroup (uint32 u) const { std::map<uint32,int32>::const_iterator it = m_kSMGroup.find(u);  return (it == m_kSMGroup.end())  ? 0 : it->second; }
int32 GBTables::SMNPC   (uint32 u) const { std::map<uint32,int32>::const_iterator it = m_kSMNPC.find(u);    return (it == m_kSMNPC.end())    ? 0 : it->second; }
int32 GBTables::EventCode(const std::string& rE) const {
    std::map<std::string,int32>::const_iterator it = m_kEvent.find(rE);
    return (it == m_kEvent.end()) ? 0 : it->second; }
int32 GBTables::Reward(uint32 uH, std::string& rI, uint32& rQ) const {
    std::map<uint32, RewardRow>::const_iterator it = m_kReward.find(uH);
    if (it == m_kReward.end()) return 0;
    rI = it->second.kItem; rQ = it->second.uiQty; return 1;
}

// -----------------------------------------------------------------------------
//  GTITables
// -----------------------------------------------------------------------------
GTITables& GTITables::Get() { static GTITables s; return s; }
void GTITables::Bind() {
    if (const ShnFile* t = T("GTIBreedSubject")) ITER(t) {
        BreedRow r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.kSubjectInx  = ShnGetStr(*t, _r, "SubjectInx");
        r.uiTier       = ShnGetU32(*t, _r, "Tier");
        m_kBreedById[r.uiID] = m_kBreed.size();
        m_kBreed.push_back(r);
    }
    if (const ShnFile* t = T("GTIGetRate")) ITER(t)
        m_kRate[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "Rate");
    if (const ShnFile* t = T("GTIGetRateGap")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "ID") << 32) | (uint32)ShnGetI32(*t, _r, "Gap");
        m_kRateGap[k] = ShnGetI32(*t, _r, "Rate");
    }
    if (const ShnFile* t = T("GTIServer")) ITER(t)
        m_kServer[ShnGetU32(*t, _r, "ID")] = ShnGetI32(*t, _r, "Cap");
    if (const ShnFile* t = T("GTWinScore")) ITER(t)
        m_kWinScore[ShnGetU32(*t, _r, "Place")] = ShnGetI32(*t, _r, "Score");
}
const GTITables::BreedRow* GTITables::FindBreed(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kBreedById.find(u);
    return (it == m_kBreedById.end()) ? NULL : &m_kBreed[it->second]; }
int32 GTITables::GetRate (uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kRate.find(u);
    return (it == m_kRate.end()) ? 0 : it->second; }
int32 GTITables::GetRateGap(uint32 u, int32 g) const {
    uint64 k = ((uint64)u << 32) | (uint32)g;
    std::map<uint64,int32>::const_iterator it = m_kRateGap.find(k);
    return (it == m_kRateGap.end()) ? 0 : it->second; }
int32 GTITables::ServerCap(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kServer.find(u);
    return (it == m_kServer.end()) ? 0 : it->second; }
int32 GTITables::WinScore(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kWinScore.find(u);
    return (it == m_kWinScore.end()) ? 0 : it->second; }

// -----------------------------------------------------------------------------
//  GuildAcademyExtraTables
// -----------------------------------------------------------------------------
GuildAcademyExtraTables& GuildAcademyExtraTables::Get() { static GuildAcademyExtraTables s; return s; }
void GuildAcademyExtraTables::Bind() {
    if (const ShnFile* t = T("GuildAcademyLevelUp")) ITER(t) {
        LevelUpRow r;
        r.uiLevel       = ShnGetU32(*t, _r, "Level");
        r.uiExpRequired = ShnGetU32(*t, _r, "ExpRequired");
        r.uiBuffID      = ShnGetU32(*t, _r, "BuffID");
        m_kLvlById[r.uiLevel] = m_kLvl.size();
        m_kLvl.push_back(r);
    }
    if (const ShnFile* t = T("GuildAcademyRank")) ITER(t) {
        RankRow r;
        r.uiRank      = ShnGetU32(*t, _r, "Rank");
        r.kInxName    = ShnGetStr(*t, _r, "InxName");
        r.uiThreshold = ShnGetU32(*t, _r, "Threshold");
        m_kRankById[r.uiRank] = m_kRank.size();
        m_kRank.push_back(r);
    }
    if (const ShnFile* t = T("GuildGradeScoreData")) ITER(t)
        m_kGradeScore[ShnGetU32(*t, _r, "Grade")] = ShnGetI32(*t, _r, "Score");
    if (const ShnFile* t = T("GuildLevelScoreData")) ITER(t)
        m_kLevelScore[ShnGetU32(*t, _r, "Level")] = ShnGetI32(*t, _r, "Score");
}
const GuildAcademyExtraTables::LevelUpRow* GuildAcademyExtraTables::FindLevel(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kLvlById.find(u);
    return (it == m_kLvlById.end()) ? NULL : &m_kLvl[it->second]; }
const GuildAcademyExtraTables::RankRow* GuildAcademyExtraTables::FindRank(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kRankById.find(u);
    return (it == m_kRankById.end()) ? NULL : &m_kRank[it->second]; }
int32 GuildAcademyExtraTables::GradeScore(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kGradeScore.find(u);
    return (it == m_kGradeScore.end()) ? 0 : it->second; }
int32 GuildAcademyExtraTables::LevelScore(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kLevelScore.find(u);
    return (it == m_kLevelScore.end()) ? 0 : it->second; }

// -----------------------------------------------------------------------------
//  GuildTournamentExtraTables
// -----------------------------------------------------------------------------
GuildTournamentExtraTables& GuildTournamentExtraTables::Get() { static GuildTournamentExtraTables s; return s; }
void GuildTournamentExtraTables::Bind() {
    if (const ShnFile* t = T("GuildTournamentLvGap")) ITER(t)
        m_kLvGap[ShnGetI32(*t, _r, "LvDiff")] = ShnGetI32(*t, _r, "Mul");
    if (const ShnFile* t = T("GuildTournamentMasterBuff")) ITER(t)
        m_kMasterBuff[ShnGetU32(*t, _r, "Tier")] = ShnGetI32(*t, _r, "BuffID");
    if (const ShnFile* t = T("GuildTournamentOccupy")) ITER(t)
        m_kOccupy[ShnGetU32(*t, _r, "HoldSec")] = ShnGetI32(*t, _r, "Points");
    if (const ShnFile* t = T("GuildTournamentRequire")) ITER(t)
        m_kRequire[ShnGetU32(*t, _r, "GTNo")] = ShnGetStr(*t, _r, "Note");
    if (const ShnFile* t = T("GuildTournamentScore")) ITER(t)
        m_kScore[ShnGetU32(*t, _r, "Place")] = ShnGetI32(*t, _r, "Score");
    if (const ShnFile* t = T("GuildTournamentSkill")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "Tier") << 32) | ShnGetU32(*t, _r, "Slot");
        m_kSkill[k] = ShnGetI32(*t, _r, "SkillID");
    }
}
int32 GuildTournamentExtraTables::LvGapMul(int32 d) const {
    std::map<int32,int32>::const_iterator it = m_kLvGap.find(d);
    return (it == m_kLvGap.end()) ? 100 : it->second; }
int32 GuildTournamentExtraTables::MasterBuffID(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kMasterBuff.find(u);
    return (it == m_kMasterBuff.end()) ? 0 : it->second; }
int32 GuildTournamentExtraTables::OccupyPoints(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kOccupy.find(u);
    return (it == m_kOccupy.end()) ? 0 : it->second; }
bool GuildTournamentExtraTables::Require(uint32 u, std::string& rN) const {
    std::map<uint32,std::string>::const_iterator it = m_kRequire.find(u);
    if (it == m_kRequire.end()) return false;
    rN = it->second; return true;
}
int32 GuildTournamentExtraTables::ScoreFor(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kScore.find(u);
    return (it == m_kScore.end()) ? 0 : it->second; }
int32 GuildTournamentExtraTables::SkillIDFor(uint32 ti, uint32 sl) const {
    uint64 k = ((uint64)ti << 32) | sl;
    std::map<uint64,int32>::const_iterator it = m_kSkill.find(k);
    return (it == m_kSkill.end()) ? 0 : it->second; }

// -----------------------------------------------------------------------------
//  ItemExtraTables
// -----------------------------------------------------------------------------
ItemExtraTables& ItemExtraTables::Get() { static ItemExtraTables s; return s; }
void ItemExtraTables::Bind() {
    if (const ShnFile* t = T("ItemDismantle")) ITER(t) {
        DismantleRow r;
        r.uiItemID    = ShnGetU32(*t, _r, "ItemID");
        r.kRewardItem = ShnGetStr(*t, _r, "RewardItem");
        r.uiMin       = ShnGetU32(*t, _r, "Min");
        r.uiMax       = ShnGetU32(*t, _r, "Max");
        m_kDisById[r.uiItemID] = m_kDis.size();
        m_kDis.push_back(r);
    }
    if (const ShnFile* t = T("ItemDropLog")) ITER(t)
        m_kDropLog[ShnGetU32(*t, _r, "ItemID")] = ShnGetI32(*t, _r, "AnnounceLevel");
    if (const ShnFile* t = T("ItemInvenDel")) ITER(t)
        m_kInvenDel[ShnGetU32(*t, _r, "ItemID")] = ShnGetI32(*t, _r, "AfterDays");
    if (const ShnFile* t = T("ItemMerchantInfo")) ITER(t) {
        MerchantRow r;
        r.kNpcInx  = ShnGetStr(*t, _r, "NpcInx");
        r.uiSlot   = ShnGetU32(*t, _r, "Slot");
        r.kItemInx = ShnGetStr(*t, _r, "ItemInx");
        r.uiPrice  = ShnGetU32(*t, _r, "Price");
        m_kMerchByNpc[r.kNpcInx].push_back(m_kMerch.size());
        m_kMerch.push_back(r);
    }
    if (const ShnFile* t = T("ItemMix")) ITER(t) {
        MixRow r;
        r.uiID    = ShnGetU32(*t, _r, "ID");
        r.kIn1    = ShnGetStr(*t, _r, "In1");
        r.kIn2    = ShnGetStr(*t, _r, "In2");
        r.kIn3    = ShnGetStr(*t, _r, "In3");
        r.kOut    = ShnGetStr(*t, _r, "Out");
        r.uiRate  = ShnGetU32(*t, _r, "Rate");
        r.uiCost  = ShnGetU32(*t, _r, "Cost");
        m_kMixById[r.uiID] = m_kMix.size();
        m_kMix.push_back(r);
    }
    if (const ShnFile* t = T("ItemMoney")) ITER(t)
        m_kMoney[ShnGetU32(*t, _r, "ItemID")] = (int64)ShnGetU32(*t, _r, "Value");
    if (const ShnFile* t = T("ItemOptions")) ITER(t) {
        OptionsRow r;
        r.uiEquipType = ShnGetU32(*t, _r, "EquipType");
        r.uiSlot      = ShnGetU32(*t, _r, "Slot");
        r.uiOptionID  = ShnGetU32(*t, _r, "OptionID");
        r.uiPercent   = ShnGetU32(*t, _r, "Percent");
        uint64 k = ((uint64)r.uiEquipType << 32) | r.uiSlot;
        m_kOptByKey[k].push_back(m_kOpt.size());
        m_kOpt.push_back(r);
    }
    if (const ShnFile* t = T("ItemPackage")) ITER(t) {
        PackageRow r;
        r.uiPackageID = ShnGetU32(*t, _r, "PackageID");
        r.kItem       = ShnGetStr(*t, _r, "Item");
        r.uiQty       = ShnGetU32(*t, _r, "Qty");
        r.uiUpgrade   = ShnGetU32(*t, _r, "Upgrade");
        m_kPkgById[r.uiPackageID].push_back(m_kPkg.size());
        m_kPkg.push_back(r);
    }
    if (const ShnFile* t = T("ItemServerEquipTypeInfo")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "EquipType") << 32) | ShnGetU32(*t, _r, "Slot");
        m_kSrvEq[k] = ShnGetU32(*t, _r, "AllowedClass");
    }
    if (const ShnFile* t = T("ItemShop")) ITER(t) {
        ShopRow r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.kItemInx     = ShnGetStr(*t, _r, "ItemInx");
        r.uiPriceVis   = ShnGetU32(*t, _r, "PriceVis");
        r.uiPriceMicro = ShnGetU32(*t, _r, "PriceMicro");
        m_kShopById[r.uiID] = m_kShop.size();
        m_kShop.push_back(r);
    }
    if (const ShnFile* t = T("ItemSort")) ITER(t) {
        SortRow r;
        r.uiItemID     = ShnGetU32(*t, _r, "ItemID");
        r.uiCategory   = ShnGetU32(*t, _r, "Category");
        r.uiSubCategory= ShnGetU32(*t, _r, "SubCategory");
        m_kSortById[r.uiItemID] = m_kSort.size();
        m_kSort.push_back(r);
    }
    if (const ShnFile* t = T("ItemUseEffect")) ITER(t) {
        UseEffectRow r;
        r.uiItemID    = ShnGetU32(*t, _r, "ItemID");
        r.uiAbStateID = ShnGetU32(*t, _r, "AbStateID");
        r.uiKeepTimeMs= ShnGetU32(*t, _r, "KeepTime");
        m_kUseById[r.uiItemID] = m_kUse.size();
        m_kUse.push_back(r);
    }
    if (const ShnFile* t = T("JobEquipInfo")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "JobID") << 32) | ShnGetU32(*t, _r, "EquipType");
        m_kJob[k] = ShnGetU32(*t, _r, "Allowed") != 0;
    }
}
const ItemExtraTables::DismantleRow* ItemExtraTables::FindDismantle(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kDisById.find(u);
    return (it == m_kDisById.end()) ? NULL : &m_kDis[it->second]; }
int32 ItemExtraTables::DropAnnounce(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kDropLog.find(u);
    return (it == m_kDropLog.end()) ? 0 : it->second; }
int32 ItemExtraTables::InvenDelDays(uint32 u) const {
    std::map<uint32,int32>::const_iterator it = m_kInvenDel.find(u);
    return (it == m_kInvenDel.end()) ? 0 : it->second; }
void ItemExtraTables::MerchantList(const std::string& rN,
                                   std::vector<const MerchantRow*>& rOut) const {
    rOut.clear();
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kMerchByNpc.find(rN);
    if (it == m_kMerchByNpc.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kMerch[it->second[i]]);
}
const ItemExtraTables::MixRow* ItemExtraTables::FindMix(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kMixById.find(u);
    return (it == m_kMixById.end()) ? NULL : &m_kMix[it->second]; }
int64 ItemExtraTables::MoneyValueOf(uint32 u) const {
    std::map<uint32,int64>::const_iterator it = m_kMoney.find(u);
    return (it == m_kMoney.end()) ? 0 : it->second; }
void ItemExtraTables::OptionsFor(uint32 e, uint32 s,
                                 std::vector<const OptionsRow*>& rOut) const {
    rOut.clear();
    uint64 k = ((uint64)e << 32) | s;
    std::map<uint64, std::vector<size_t> >::const_iterator it = m_kOptByKey.find(k);
    if (it == m_kOptByKey.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kOpt[it->second[i]]);
}
void ItemExtraTables::PackageContents(uint32 u, std::vector<const PackageRow*>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<size_t> >::const_iterator it = m_kPkgById.find(u);
    if (it == m_kPkgById.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kPkg[it->second[i]]);
}
bool ItemExtraTables::CanEquip(uint32 e, uint32 s, uint32 c) const {
    uint64 k = ((uint64)e << 32) | s;
    std::map<uint64,uint32>::const_iterator it = m_kSrvEq.find(k);
    if (it == m_kSrvEq.end()) return true;
    return ((1u << (c & 31)) & it->second) != 0;
}
const ItemExtraTables::ShopRow* ItemExtraTables::FindShop(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kShopById.find(u);
    return (it == m_kShopById.end()) ? NULL : &m_kShop[it->second]; }
const ItemExtraTables::SortRow* ItemExtraTables::FindSort(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kSortById.find(u);
    return (it == m_kSortById.end()) ? NULL : &m_kSort[it->second]; }
const ItemExtraTables::UseEffectRow* ItemExtraTables::FindUseEffect(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kUseById.find(u);
    return (it == m_kUseById.end()) ? NULL : &m_kUse[it->second]; }
bool ItemExtraTables::JobCanEquip(uint32 j, uint32 e) const {
    uint64 k = ((uint64)j << 32) | e;
    std::map<uint64,bool>::const_iterator it = m_kJob.find(k);
    return (it == m_kJob.end()) ? true : it->second;
}

// -----------------------------------------------------------------------------
//  KQExtraTables
// -----------------------------------------------------------------------------
KQExtraTables& KQExtraTables::Get() { static KQExtraTables s; return s; }
void KQExtraTables::Bind() {
    if (const ShnFile* t = T("KQIsVote")) ITER(t)
        m_kVote[ShnGetU32(*t, _r, "KQID")] = ShnGetU32(*t, _r, "IsVote") != 0;
    if (const ShnFile* t = T("KQVoteMajorityRate")) ITER(t)
        m_kRate[ShnGetU32(*t, _r, "KQID")] = ShnGetU32(*t, _r, "MajorityRate");
    if (const ShnFile* t = T("KQTeam")) ITER(t)
        m_kTeam[ShnGetU32(*t, _r, "KQID")] = ShnGetU32(*t, _r, "TeamSize");
    if (const ShnFile* t = T("KQItem")) ITER(t)
        m_kItems[ShnGetU32(*t, _r, "KQID")].push_back(ShnGetStr(*t, _r, "ItemInx"));
}
bool KQExtraTables::IsVote(uint32 u) const {
    std::map<uint32,bool>::const_iterator it = m_kVote.find(u);
    return (it == m_kVote.end()) ? false : it->second; }
int32 KQExtraTables::VoteMajorityRate(uint32 u) const {
    std::map<uint32,uint32>::const_iterator it = m_kRate.find(u);
    return (it == m_kRate.end()) ? 50 : (int32)it->second; }
int32 KQExtraTables::TeamSize(uint32 u) const {
    std::map<uint32,uint32>::const_iterator it = m_kTeam.find(u);
    return (it == m_kTeam.end()) ? 0 : (int32)it->second; }
void KQExtraTables::Items(uint32 u, std::vector<std::string>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<std::string> >::const_iterator it = m_kItems.find(u);
    if (it != m_kItems.end()) rOut = it->second;
}

// -----------------------------------------------------------------------------
//  LuckyCoinTables
// -----------------------------------------------------------------------------
LuckyCoinTables& LuckyCoinTables::Get() { static LuckyCoinTables s; return s; }
void LuckyCoinTables::Bind() {
    if (const ShnFile* t = T("LCGroupRate")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "GroupID") << 32) | ShnGetU32(*t, _r, "Tier");
        m_kRate[k] = ShnGetU32(*t, _r, "Rate");
    }
    if (const ShnFile* t = T("LCReward")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "GroupID") << 32) | ShnGetU32(*t, _r, "Tier");
        RewardRow rr;
        rr.kItem = ShnGetStr(*t, _r, "RewardItem");
        rr.uiQty = ShnGetU32(*t, _r, "Qty");
        m_kReward[k] = rr;
    }
}
int32 LuckyCoinTables::GroupRate(uint32 g, uint32 ti) const {
    uint64 k = ((uint64)g << 32) | ti;
    std::map<uint64,uint32>::const_iterator it = m_kRate.find(k);
    return (it == m_kRate.end()) ? 0 : (int32)it->second; }
bool LuckyCoinTables::Roll(uint32 g, uint32 ti, std::string& rI, uint32& rQ) const {
    uint64 k = ((uint64)g << 32) | ti;
    std::map<uint64, RewardRow>::const_iterator it = m_kReward.find(k);
    if (it == m_kReward.end()) return false;
    rI = it->second.kItem; rQ = it->second.uiQty; return true;
}

// -----------------------------------------------------------------------------
//  MapExtraTables
// -----------------------------------------------------------------------------
MapExtraTables& MapExtraTables::Get() { static MapExtraTables s; return s; }
void MapExtraTables::Bind() {
    if (const ShnFile* t = T("MapBuff")) ITER(t) {
        BuffRow r;
        r.uiMapID    = ShnGetU32(*t, _r, "MapID");
        r.uiAbStateID= ShnGetU32(*t, _r, "AbStateID");
        r.uiCondition= ShnGetU32(*t, _r, "Condition");
        m_kBuffById[r.uiMapID] = m_kBuff.size();
        m_kBuff.push_back(r);
    }
    if (const ShnFile* t = T("MapLinkPoint")) ITER(t) {
        LinkPointRow r;
        r.uiMapID  = ShnGetU32(*t, _r, "MapID");
        r.uiLinkID = ShnGetU32(*t, _r, "LinkID");
        r.iX = ShnGetI32(*t, _r, "X"); r.iY = ShnGetI32(*t, _r, "Y"); r.iZ = ShnGetI32(*t, _r, "Z");
        r.uiToMapID= ShnGetU32(*t, _r, "ToMapID");
        r.iToX = ShnGetI32(*t, _r, "ToX"); r.iToY = ShnGetI32(*t, _r, "ToY"); r.iToZ = ShnGetI32(*t, _r, "ToZ");
        uint64 k = ((uint64)r.uiMapID << 32) | r.uiLinkID;
        m_kLinkByKey[k] = m_kLink.size();
        m_kLink.push_back(r);
    }
    if (const ShnFile* t = T("MapWayPoint")) ITER(t) {
        WayPointRow r;
        r.uiMapID = ShnGetU32(*t, _r, "MapID");
        r.uiWayID = ShnGetU32(*t, _r, "WayID");
        r.iX = ShnGetI32(*t, _r, "X"); r.iY = ShnGetI32(*t, _r, "Y"); r.iZ = ShnGetI32(*t, _r, "Z");
        m_kWayByMap[r.uiMapID].push_back(m_kWay.size());
        m_kWay.push_back(r);
    }
}
const MapExtraTables::BuffRow* MapExtraTables::FindBuff(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kBuffById.find(u);
    return (it == m_kBuffById.end()) ? NULL : &m_kBuff[it->second]; }
const MapExtraTables::LinkPointRow* MapExtraTables::FindLink(uint32 m, uint32 l) const {
    uint64 k = ((uint64)m << 32) | l;
    std::map<uint64,size_t>::const_iterator it = m_kLinkByKey.find(k);
    return (it == m_kLinkByKey.end()) ? NULL : &m_kLink[it->second]; }
void MapExtraTables::WaysFor(uint32 u, std::vector<const WayPointRow*>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<size_t> >::const_iterator it = m_kWayByMap.find(u);
    if (it == m_kWayByMap.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kWay[it->second[i]]);
}

// -----------------------------------------------------------------------------
//  MarketSearchInfoTable
// -----------------------------------------------------------------------------
MarketSearchInfoTable& MarketSearchInfoTable::Get() { static MarketSearchInfoTable s; return s; }
void MarketSearchInfoTable::Bind() {
    if (const ShnFile* t = T("MarketSearchInfo")) ITER(t)
        m_kRow[ShnGetStr(*t, _r, "Keyword")] = ShnGetU32(*t, _r, "ID");
}
int32 MarketSearchInfoTable::KeywordIdOf(const std::string& rK) const {
    std::map<std::string,uint32>::const_iterator it = m_kRow.find(rK);
    return (it == m_kRow.end()) ? 0 : (int32)it->second;
}

// -----------------------------------------------------------------------------
//  MinimonTables
// -----------------------------------------------------------------------------
MinimonTables& MinimonTables::Get() { static MinimonTables s; return s; }
void MinimonTables::Bind() {
    if (const ShnFile* t = T("MinimonInfo")) ITER(t) {
        InfoRow r;
        r.uiID       = ShnGetU32(*t, _r, "ID");
        r.kInxName   = ShnGetStr(*t, _r, "InxName");
        r.uiHP       = ShnGetU32(*t, _r, "HP");
        r.uiAtk      = ShnGetU32(*t, _r, "Atk");
        r.uiDef      = ShnGetU32(*t, _r, "Def");
        r.uiCDms     = ShnGetU32(*t, _r, "CD");
        r.uiSpawnSec = ShnGetU32(*t, _r, "SpawnSec");
        m_kInfoById[r.uiID] = m_kInfo.size();
        m_kInfo.push_back(r);
    }
    if (const ShnFile* t = T("MinimonAutoUseItem")) ITER(t) {
        AutoRow r;
        r.uiMinimonID    = ShnGetU32(*t, _r, "MinimonID");
        r.kItemInx       = ShnGetStr(*t, _r, "ItemInx");
        r.uiThresholdPct = ShnGetU32(*t, _r, "ThresholdPct");
        m_kAutoById[r.uiMinimonID] = m_kAuto.size();
        m_kAuto.push_back(r);
    }
}
const MinimonTables::InfoRow* MinimonTables::Find(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kInfoById.find(u);
    return (it == m_kInfoById.end()) ? NULL : &m_kInfo[it->second]; }
const MinimonTables::AutoRow* MinimonTables::AutoFor(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kAutoById.find(u);
    return (it == m_kAutoById.end()) ? NULL : &m_kAuto[it->second]; }

// -----------------------------------------------------------------------------
//  MobExtraTables
// -----------------------------------------------------------------------------
MobExtraTables& MobExtraTables::Get() { static MobExtraTables s; return s; }
void MobExtraTables::Bind() {
    if (const ShnFile* t = T("MobAbStateDropSetting")) ITER(t)
        m_kDropSet[ShnGetU32(*t, _r, "AbState")] = ShnGetU32(*t, _r, "Percent");
    if (const ShnFile* t = T("MobConditionServer")) ITER(t) {
        CondRow r;
        r.kMobInx = ShnGetStr(*t, _r, "MobInx");
        r.uiCondID= ShnGetU32(*t, _r, "CondID");
        r.kArg    = ShnGetStr(*t, _r, "Arg");
        m_kCondByMob[r.kMobInx].push_back(m_kCond.size());
        m_kCond.push_back(r);
    }
    if (const ShnFile* t = T("MobKillAble")) ITER(t) {
        KillAbleRow r;
        r.kMobInx = ShnGetStr(*t, _r, "MobInx");
        r.uiMinLv = ShnGetU32(*t, _r, "MinLv");
        r.uiMaxLv = ShnGetU32(*t, _r, "MaxLv");
        m_kKillByMob[r.kMobInx] = m_kKill.size();
        m_kKill.push_back(r);
    }
    if (const ShnFile* t = T("MobKillAnnounce")) ITER(t) {
        AnnounceRow r;
        r.kMobInx  = ShnGetStr(*t, _r, "MobInx");
        r.uiColor  = ShnGetU32(*t, _r, "Color");
        r.kMessage = ShnGetStr(*t, _r, "Message");
        m_kAnnByMob[r.kMobInx] = m_kAnn.size();
        m_kAnn.push_back(r);
    }
    if (const ShnFile* t = T("MobKillLog")) ITER(t)
        m_kLog[ShnGetStr(*t, _r, "MobInx")] = true;
    if (const ShnFile* t = T("MobNoFadeIn")) ITER(t)
        m_kFade[ShnGetStr(*t, _r, "MobInx")] = true;
    if (const ShnFile* t = T("MobRegenAni")) ITER(t)
        m_kRegenAni[ShnGetStr(*t, _r, "MobInx")] = ShnGetU32(*t, _r, "AniID");
}
int32 MobExtraTables::DropPctForAbState(uint32 u) const {
    std::map<uint32,uint32>::const_iterator it = m_kDropSet.find(u);
    return (it == m_kDropSet.end()) ? 0 : (int32)it->second; }
void MobExtraTables::CondsFor(const std::string& rM, std::vector<const CondRow*>& rOut) const {
    rOut.clear();
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kCondByMob.find(rM);
    if (it == m_kCondByMob.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kCond[it->second[i]]);
}
const MobExtraTables::KillAbleRow* MobExtraTables::FindKillAble(const std::string& rM) const {
    std::map<std::string,size_t>::const_iterator it = m_kKillByMob.find(rM);
    return (it == m_kKillByMob.end()) ? NULL : &m_kKill[it->second]; }
const MobExtraTables::AnnounceRow* MobExtraTables::FindAnnounce(const std::string& rM) const {
    std::map<std::string,size_t>::const_iterator it = m_kAnnByMob.find(rM);
    return (it == m_kAnnByMob.end()) ? NULL : &m_kAnn[it->second]; }
bool MobExtraTables::IsLogged(const std::string& rM) const {
    std::map<std::string,bool>::const_iterator it = m_kLog.find(rM);
    return (it == m_kLog.end()) ? false : it->second; }
bool MobExtraTables::NoFadeIn(const std::string& rM) const {
    std::map<std::string,bool>::const_iterator it = m_kFade.find(rM);
    return (it == m_kFade.end()) ? false : it->second; }
int32 MobExtraTables::RegenAniIDFor(const std::string& rM) const {
    std::map<std::string,uint32>::const_iterator it = m_kRegenAni.find(rM);
    return (it == m_kRegenAni.end()) ? 0 : (int32)it->second; }

// -----------------------------------------------------------------------------
//  MoverExtraTables
// -----------------------------------------------------------------------------
MoverExtraTables& MoverExtraTables::Get() { static MoverExtraTables s; return s; }
void MoverExtraTables::Bind() {
    if (const ShnFile* t = T("MoverHG")) ITER(t) {
        HGRow r;
        r.uiID     = ShnGetU32(*t, _r, "ID");
        r.kInxName = ShnGetStr(*t, _r, "InxName");
        r.uiGrade  = ShnGetU32(*t, _r, "Grade");
        m_kHGById[r.uiID] = m_kHG.size();
        m_kHG.push_back(r);
    }
    if (const ShnFile* t = T("MoverUpgradeEffect")) ITER(t) {
        UpEffRow r;
        r.uiID       = ShnGetU32(*t, _r, "ID");
        r.uiEffectID = ShnGetU32(*t, _r, "EffectID");
        r.iValue     = ShnGetI32(*t, _r, "Value");
        m_kUpById[r.uiID] = m_kUp.size();
        m_kUp.push_back(r);
    }
    if (const ShnFile* t = T("MoverUseSkill")) ITER(t) {
        UseSkillRow r;
        r.uiMoverID = ShnGetU32(*t, _r, "MoverID");
        r.uiSlot    = ShnGetU32(*t, _r, "Slot");
        r.kSkillInx = ShnGetStr(*t, _r, "SkillInx");
        m_kSkillByMover[r.uiMoverID].push_back(m_kSkill.size());
        m_kSkill.push_back(r);
    }
    if (const ShnFile* t = T("RareMoverEachRate")) ITER(t)
        m_kEach[ShnGetU32(*t, _r, "MoverID")] = ShnGetU32(*t, _r, "Rate");
    if (const ShnFile* t = T("RareMoverSubRate")) ITER(t) {
        uint64 k = ((uint64)ShnGetU32(*t, _r, "MoverID") << 32) | ShnGetU32(*t, _r, "SubID");
        m_kSub[k] = ShnGetU32(*t, _r, "Rate");
    }
}
const MoverExtraTables::HGRow* MoverExtraTables::FindHG(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kHGById.find(u);
    return (it == m_kHGById.end()) ? NULL : &m_kHG[it->second]; }
const MoverExtraTables::UpEffRow* MoverExtraTables::FindUpEff(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kUpById.find(u);
    return (it == m_kUpById.end()) ? NULL : &m_kUp[it->second]; }
void MoverExtraTables::SkillsFor(uint32 u, std::vector<const UseSkillRow*>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<size_t> >::const_iterator it = m_kSkillByMover.find(u);
    if (it == m_kSkillByMover.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kSkill[it->second[i]]);
}
int32 MoverExtraTables::RareRate(uint32 u) const {
    std::map<uint32,uint32>::const_iterator it = m_kEach.find(u);
    return (it == m_kEach.end()) ? 0 : (int32)it->second; }
int32 MoverExtraTables::SubRate(uint32 m, uint32 s) const {
    uint64 k = ((uint64)m << 32) | s;
    std::map<uint64,uint32>::const_iterator it = m_kSub.find(k);
    return (it == m_kSub.end()) ? 0 : (int32)it->second; }

// -----------------------------------------------------------------------------
//  MsgWorldManager / MultiHitType / MysteryVault / NpcSchedule
// -----------------------------------------------------------------------------
MsgWorldManagerTable& MsgWorldManagerTable::Get() { static MsgWorldManagerTable s; return s; }
void MsgWorldManagerTable::Bind() {
    if (const ShnFile* t = T("MsgWorldManager")) ITER(t)
        m_kRow[ShnGetU32(*t, _r, "ID")] = ShnGetStr(*t, _r, "Message");
}
const std::string& MsgWorldManagerTable::Get(uint32 u) const {
    std::map<uint32,std::string>::const_iterator it = m_kRow.find(u);
    return (it == m_kRow.end()) ? m_kEmpty : it->second; }

MultiHitTypeTable& MultiHitTypeTable::Get() { static MultiHitTypeTable s; return s; }
void MultiHitTypeTable::Bind() {
    if (const ShnFile* t = T("MultiHitType")) ITER(t) {
        Row r;
        r.uiTypeID         = ShnGetU32(*t, _r, "TypeID");
        r.uiTickMs         = ShnGetU32(*t, _r, "TickMs");
        r.uiCount          = ShnGetU32(*t, _r, "Count");
        r.uiAttenuationPct = ShnGetU32(*t, _r, "AttenuationPct");
        m_kById[r.uiTypeID] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const MultiHitTypeTable::Row* MultiHitTypeTable::Find(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kById.find(u);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second]; }

MysteryVaultTable& MysteryVaultTable::Get() { static MysteryVaultTable s; return s; }
void MysteryVaultTable::Bind() {
    if (const ShnFile* t = T("MysteryVaultServer")) ITER(t) {
        Row r;
        r.uiVaultID    = ShnGetU32(*t, _r, "VaultID");
        r.kRewardItem  = ShnGetStr(*t, _r, "RewardItem");
        r.uiQty        = ShnGetU32(*t, _r, "Qty");
        r.uiPercent    = ShnGetU32(*t, _r, "Percent");
        m_kByVault[r.uiVaultID].push_back(m_kRows.size());
        m_kRows.push_back(r);
    }
}
void MysteryVaultTable::RowsFor(uint32 u, std::vector<const Row*>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<size_t> >::const_iterator it = m_kByVault.find(u);
    if (it == m_kByVault.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kRows[it->second[i]]);
}
bool MysteryVaultTable::Roll(uint32 u, std::string& rI, uint32& rQ) const {
    std::vector<const Row*> rows;
    RowsFor(u, rows);
    if (rows.empty()) return false;
    // Sum percent and roll. (Caller seeds rand().)
    uint32 total = 0;
    for (size_t i = 0; i < rows.size(); ++i) total += rows[i]->uiPercent;
    if (total == 0) total = 1;
    uint32 dice = (uint32)(rand() % total);
    uint32 acc = 0;
    for (size_t i = 0; i < rows.size(); ++i) {
        acc += rows[i]->uiPercent;
        if (dice < acc) { rI = rows[i]->kRewardItem; rQ = rows[i]->uiQty; return true; }
    }
    rI = rows.back()->kRewardItem; rQ = rows.back()->uiQty; return true;
}

NpcScheduleTable& NpcScheduleTable::Get() { static NpcScheduleTable s; return s; }
void NpcScheduleTable::Bind() {
    if (const ShnFile* t = T("NpcSchedule")) ITER(t) {
        Row r;
        r.kNpcInx    = ShnGetStr(*t, _r, "NpcInx");
        r.uiStartHour= ShnGetU32(*t, _r, "StartHour");
        r.uiEndHour  = ShnGetU32(*t, _r, "EndHour");
        r.uiDayMask  = ShnGetU32(*t, _r, "DayMask");
        m_kByNpc[r.kNpcInx].push_back(m_kRows.size());
        m_kRows.push_back(r);
    }
}
bool NpcScheduleTable::IsActive(const std::string& rN, uint32 uH) const {
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kByNpc.find(rN);
    if (it == m_kByNpc.end()) return true;     // no schedule -> always active
    uint32 dow = (uH / 24) % 7;
    uint32 hod = uH % 24;
    for (size_t i = 0; i < it->second.size(); ++i) {
        const Row& r = m_kRows[it->second[i]];
        if (((r.uiDayMask >> dow) & 1) == 0) continue;
        if (hod >= r.uiStartHour && hod < r.uiEndHour) return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
//  PSkillSetAbstate / QuestData / Attendance
// -----------------------------------------------------------------------------
PSkillSetAbstateTable& PSkillSetAbstateTable::Get() { static PSkillSetAbstateTable s; return s; }
void PSkillSetAbstateTable::Bind() {
    if (const ShnFile* t = T("PSkillSetAbstate")) ITER(t)
        m_kRow[ShnGetU32(*t, _r, "PSkillID")] = ShnGetU32(*t, _r, "AbStateID");
}
int32 PSkillSetAbstateTable::AbStateFor(uint32 u) const {
    std::map<uint32,uint32>::const_iterator it = m_kRow.find(u);
    return (it == m_kRow.end()) ? 0 : (int32)it->second;
}

QuestDataTable& QuestDataTable::Get() { static QuestDataTable s; return s; }
void QuestDataTable::Bind() {
    if (const ShnFile* t = T("QuestData")) ITER(t) {
        Row r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.kInxName     = ShnGetStr(*t, _r, "InxName");
        r.uiMinLv      = ShnGetU32(*t, _r, "MinLv");
        r.uiMaxLv      = ShnGetU32(*t, _r, "MaxLv");
        r.uiTimeLimitSec= ShnGetU32(*t, _r, "TimeLimit");
        r.uiMaxRepeat  = ShnGetU32(*t, _r, "MaxRepeat");
        m_kById[r.uiID] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const QuestDataTable::Row* QuestDataTable::Find(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kById.find(u);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second]; }

AttendanceTables& AttendanceTables::Get() { static AttendanceTables s; return s; }
void AttendanceTables::Bind() {
    if (const ShnFile* t = T("AttendSchedule")) ITER(t) {
        ScheduleRow r;
        r.uiDay       = ShnGetU32(*t, _r, "Day");
        r.kRewardItem = ShnGetStr(*t, _r, "RewardItem");
        r.uiQty       = ShnGetU32(*t, _r, "Qty");
        m_kSchedByDay[r.uiDay] = m_kSched.size();
        m_kSched.push_back(r);
    }
    if (const ShnFile* t = T("AttendReward")) ITER(t)
        m_kRewardItemQty[ShnGetStr(*t, _r, "ItemInx")] = ShnGetU32(*t, _r, "Qty");
}
const AttendanceTables::ScheduleRow* AttendanceTables::OnDay(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kSchedByDay.find(u);
    return (it == m_kSchedByDay.end()) ? NULL : &m_kSched[it->second]; }
int32 AttendanceTables::RewardItemQty(const std::string& rI) const {
    std::map<std::string, uint32>::const_iterator it = m_kRewardItemQty.find(rI);
    return (it == m_kRewardItemQty.end()) ? 0 : (int32)it->second;
}

// -----------------------------------------------------------------------------
//  NPCDialogTables
// -----------------------------------------------------------------------------
NPCDialogTables& NPCDialogTables::Get() { static NPCDialogTables s; return s; }
void NPCDialogTables::Bind() {
    if (const ShnFile* t = T("NpcDialogData")) ITER(t) {
        DialogRow r;
        r.uiDialogID     = ShnGetU32(*t, _r, "DialogID");
        if (r.uiDialogID == 0) r.uiDialogID = ShnGetU32(*t, _r, "ID");
        r.kTextKey       = ShnGetStr(*t, _r, "Dialog");
        if (r.kTextKey.empty()) r.kTextKey = ShnGetStr(*t, _r, "Text");
        r.kButtonKeys    = ShnGetStr(*t, _r, "ButtonKeys");
        if (r.kButtonKeys.empty()) r.kButtonKeys = ShnGetStr(*t, _r, "Buttons");
        r.uiNextDialogID = ShnGetU32(*t, _r, "NextDialogID");
        m_kDialogById[r.uiDialogID] = m_kDialog.size();
        m_kDialog.push_back(r);
    }
    if (const ShnFile* t = T("NPCViewInfo")) ITER(t) {
        ViewRow r;
        r.uiViewInfoID = ShnGetU32(*t, _r, "ViewInfoID");
        if (r.uiViewInfoID == 0) r.uiViewInfoID = ShnGetU32(*t, _r, "ID");
        r.kLabelKey    = ShnGetStr(*t, _r, "Label");
        if (r.kLabelKey.empty()) r.kLabelKey = ShnGetStr(*t, _r, "Name");
        r.uiIconID     = ShnGetU32(*t, _r, "IconID");
        r.kActionTag   = ShnGetStr(*t, _r, "Action");
        r.kArg0        = ShnGetStr(*t, _r, "Arg0");
        r.kArg1        = ShnGetStr(*t, _r, "Arg1");
        m_kViewById[r.uiViewInfoID] = m_kView.size();
        m_kView.push_back(r);
    }
}
const NPCDialogTables::DialogRow* NPCDialogTables::FindDialog(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kDialogById.find(u);
    return (it == m_kDialogById.end()) ? NULL : &m_kDialog[it->second]; }
const NPCDialogTables::ViewRow* NPCDialogTables::FindView(uint32 u) const {
    std::map<uint32,size_t>::const_iterator it = m_kViewById.find(u);
    return (it == m_kViewById.end()) ? NULL : &m_kView[it->second]; }

void NPCDialogTables::ButtonsFor(uint32 uiDialogID,
                                 std::vector<const ViewRow*>& rOut) const {
    rOut.clear();
    const DialogRow* pkD = FindDialog(uiDialogID);
    if (!pkD) return;
    // Comma-separated ViewInfoID list.
    const std::string& s = pkD->kButtonKeys;
    size_t i = 0;
    while (i < s.size()) {
        // skip whitespace and commas
        while (i < s.size() && (s[i] == ',' || s[i] == ' ')) ++i;
        size_t j = i;
        while (j < s.size() && s[j] != ',' && s[j] != ' ') ++j;
        if (j > i) {
            std::string tok = s.substr(i, j - i);
            uint32 id = (uint32)strtoul(tok.c_str(), NULL, 10);
            if (id) {
                const ViewRow* pkV = FindView(id);
                if (pkV) rOut.push_back(pkV);
            }
        }
        i = j;
    }
}

// -----------------------------------------------------------------------------
//  One-call binder
// -----------------------------------------------------------------------------
void BindAllExtendedTables() {
    AccUpgradeTables::Get()              .Bind();
    ActionExtraTables::Get()             .Bind();
    AdminLvSetTable::Get()               .Bind();
    AnnounceDataTable::Get()             .Bind();
    BMPTable::Get()                      .Bind();
    BelongTables::Get()                  .Bind();
    CharTitleStateServerTable::Get()     .Bind();
    ChatColorTable::Get()                .Bind();
    ColorInfoTables::Get()               .Bind();
    CollectExtrasTables::Get()           .Bind();
    DiceTables::Get()                    .Bind();
    EnchantSocketTable::Get()            .Bind();
    ErrorCodeTable::Get()                .Bind();
    EstateExtraTables::Get()             .Bind();
    FieldLvConditionTable::Get()         .Bind();
    FriendPointRewardTable::Get()        .Bind();
    SetItemEffectTable::Get()            .Bind();
    GroupAbStateTable::Get()             .Bind();
    GBTables::Get()                      .Bind();
    GTITables::Get()                     .Bind();
    GuildAcademyExtraTables::Get()       .Bind();
    GuildTournamentExtraTables::Get()    .Bind();
    ItemExtraTables::Get()               .Bind();
    KQExtraTables::Get()                 .Bind();
    LuckyCoinTables::Get()               .Bind();
    MapExtraTables::Get()                .Bind();
    MarketSearchInfoTable::Get()         .Bind();
    MinimonTables::Get()                 .Bind();
    MobExtraTables::Get()                .Bind();
    MoverExtraTables::Get()              .Bind();
    MsgWorldManagerTable::Get()          .Bind();
    MultiHitTypeTable::Get()             .Bind();
    MysteryVaultTable::Get()             .Bind();
    NpcScheduleTable::Get()              .Bind();
    PSkillSetAbstateTable::Get()         .Bind();
    QuestDataTable::Get()                .Bind();
    AttendanceTables::Get()              .Bind();
    NPCDialogTables ::Get()              .Bind();

    SHINELOG_INFO("BindAllExtendedTables: 110+ long-tail SHN accessors bound");
}

#undef ITER
} // namespace fiesta
