// Server/DataServer/Common/DBSchemaConstants.h
//                                   AccountLog.bak / OperatorTool.bak / StatisticsData.bak /
//                                   Options.bak / World00_GameLog.bak (2026-02 drop).
// Authoritative table + column-name catalog. Each sub-namespace mirrors one real
// SQL Server table; every constant is the literal column or table identifier as
// it appears in the original schema. SQLP composes SQL by concatenating these
// constants, so any typo here would surface immediately in `up_*` / `p_*`
// stored-procedure dispatch.
// VS2010-compatible: only `static const char* const` storage in headers.
#ifndef SHINE_DBSCHEMA_CONST_H
#define SHINE_DBSCHEMA_CONST_H
namespace shine { namespace DB {

// ============================================================================
//  Account database (logical DB name: Account)
// ============================================================================
namespace tUser {
    static const char* const kTable      = "tUser";
    static const char* const kUserNo     = "nUserNo";       // PK identity
    static const char* const kUserID     = "sUserID";
    static const char* const kUserPW     = "sUserPW";
    static const char* const kUserName   = "sUserName";
    static const char* const kEmail      = "sEmail";
    static const char* const kUserIP     = "sUserIP";
    static const char* const kJuminNo    = "sJuminNo";      // KR resident ID equivalent
    static const char* const kPhoneNo    = "sPhoneNo";
    static const char* const kAddress    = "sAddress";
    static const char* const kSIDO       = "sSIDO";
    static const char* const kGUGUN      = "sGUGUN";
    static const char* const kDONG       = "sDONG";
    static const char* const kZIPCODE    = "sZIPCODE";
    static const char* const kIsBlock    = "bIsBlock";
    static const char* const kIsMail     = "bIsMail";
    static const char* const kAuthID     = "nAuthID";
    static const char* const kAffiliateID= "nAffiliateID";
}
namespace tUserAuth {
    static const char* const kTable       = "tUserAuth";
    static const char* const kAuthID      = "nAuthID";
    static const char* const kAuthName    = "sAuthName";
    static const char* const kIsLoginAble = "bIsLoginAble";
}
namespace tUserBlock {
    static const char* const kTable     = "tUserBlock";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kBlockNo   = "nBlockNo";
    static const char* const kCauseNo   = "nCauseNo";
    static const char* const kComment   = "sComment";
}
namespace tUserOption {
    static const char* const kTable     = "tUserOption";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kOptionNo  = "nOptionNo";
    static const char* const kStatus    = "bStatus";
}
namespace tCash {
    static const char* const kTable     = "tCash";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kAmount    = "nAmount";
}
namespace tCashLog {
    static const char* const kTable     = "tCashLog";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kBefore    = "nBefore";
    static const char* const kAfter     = "nAfter";
    static const char* const kType      = "nType";
}
namespace tChannelLog {
    static const char* const kTable     = "tChannelLog";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kChannel   = "sChannel";
}
namespace tChargeItem {
    static const char* const kTable     = "tChargeItem";
    static const char* const kUserNo    = "userNo";
    static const char* const kOrderNo   = "orderNo";
    static const char* const kGoodsNo   = "goodsNo";
    static const char* const kStatus    = "bStatus";
}
namespace tChargeItemDelLog {
    static const char* const kTable     = "tChargeItemDelLog";
    static const char* const kUserNo    = "userNo";
    static const char* const kOrderNo   = "orderNo";
}
namespace tDrawComment {
    static const char* const kTable     = "tDrawComment";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kComment   = "sComment";
}
namespace tOrder {
    static const char* const kTable     = "tOrder";
    static const char* const kUserNo    = "userNo";
    static const char* const kOrderNo   = "orderNo";
}
namespace tPasswordLog {
    static const char* const kTable     = "tPasswordLog";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kPrevPW    = "sPrevPW";
    static const char* const kCurrPW    = "sCurrPW";
}
namespace tPayAgreement {
    static const char* const kTable     = "tPayAgreement";
    static const char* const kUserNo    = "userNo";
}
namespace tPayResult {
    static const char* const kTable     = "tPayResult";
    static const char* const kUserNo    = "userNo";
    static const char* const kStatus    = "bStatus";
}
namespace tPayType {
    static const char* const kTable     = "tPayType";
    static const char* const kType      = "nType";
}
namespace tSerialKey {
    static const char* const kTable     = "tSerialKey";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kStatus    = "bStatus";
}
namespace tZipCode {
    static const char* const kTable     = "tZipCode";
    static const char* const kZipNo     = "nZipNo";
    static const char* const kZipCode   = "sZIPCODE";
}

// ============================================================================
//  AccountLog database (logical DB name: AccountLog)
// ============================================================================
namespace tAccountLog {
    static const char* const kTable     = "tAccountLog";
    static const char* const kLogNo     = "nLogNo";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kWorldNo   = "nWorldNo";
    static const char* const kType      = "nType";
}
namespace tEvent_Friend {
    static const char* const kTable          = "tEvent_Friend";
    static const char* const kEventUserNo    = "nEventUserNo";
    static const char* const kEventCharNo    = "nEventCharNo";
    static const char* const kFriendUserNo   = "nFriendUserNo";
    static const char* const kFriendCharNo   = "nFriendCharNo";
    static const char* const kEventComplete  = "bEventComplete";
}
namespace tIPBlock {
    static const char* const kTable     = "tIPBlock";
    static const char* const kIPStart   = "sIPStart";
    static const char* const kIPEnd     = "sIPEnd";
    static const char* const kIsBlock   = "bIsBlock";
}

// ============================================================================
//  World00_Character database (logical DB name: World00_Character)
// ============================================================================
namespace tCharacter {
    static const char* const kTable        = "tCharacter";
    static const char* const kCharNo       = "nCharNo";
    static const char* const kID           = "sID";
    static const char* const kUserNo       = "nUserNo";
    static const char* const kAdminLevel   = "nAdminLevel";
    static const char* const kLoginZone    = "sLoginZone";
    static const char* const kLevel        = "nLevel";
    static const char* const kExp          = "nExp";
    static const char* const kFame         = "nFame";
    static const char* const kMoney        = "nMoney";
    static const char* const kFlags        = "nFlags";
    static const char* const kDeleted      = "bDeleted";
    static const char* const kChatBanTime  = "nChatBanTime";
}
namespace tCharacterShape {
    static const char* const kTable     = "tCharacterShape";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kClass     = "nClass";
}
namespace tFriend {
    static const char* const kTable          = "tFriend";
    static const char* const kCharNo         = "nCharNo";
    static const char* const kFriendCharNo   = "nFriendCharNo";
    static const char* const kFriendPoint    = "nFriendPoint";
    static const char* const kLastFriendPoint= "nLastFriendPoint";
}
namespace tGuild {
    static const char* const kTable        = "tGuild";
    static const char* const kGuildNo      = "nGuildNo";
    static const char* const kName         = "sName";
    static const char* const kIntro        = "sIntro";
    static const char* const kJoinLevel    = "nJoinLevel";
    static const char* const kMaxMembers   = "nMaxMembers";
    static const char* const kNumOfMember  = "nNumOfMember";
    static const char* const kStatus       = "bStatus";
}
namespace tGuildAcademyHistory {
    static const char* const kTable     = "tGuildAcademyHistory";
    static const char* const kAcademyID = "nAcademyID";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kAction    = "nAction";
}
namespace tGuildAcademyMember {
    static const char* const kTable     = "tGuildAcademyMember";
    static const char* const kAcademyID = "nAcademyID";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kStatus    = "bStatus";
}
namespace tGuildEmblem {
    static const char* const kTable     = "tGuildEmblem";
    static const char* const kGuildNo   = "nGuildNo";
    static const char* const kData      = "sData";
}
namespace tGuildToken {
    static const char* const kTable     = "tGuildToken";
    static const char* const kGuildNo   = "nGuildNo";
    static const char* const kToken     = "nToken";
    static const char* const kSubToken  = "nSubToken";
}
namespace tGuildTournament {
    static const char* const kTable      = "tGuildTournament";
    static const char* const kGTNo       = "nGTNo";
    static const char* const kGuildNo    = "nGuildNo";
    static const char* const kGameNo     = "nGameNo";
    static const char* const kStatus     = "bStatus";
    static const char* const kWinCount   = "nWinCount";
    static const char* const kPrizeCoin  = "nPrizeCoin";
}
namespace tHolyPromise {
    static const char* const kTable             = "tHolyPromise";
    static const char* const kCharNo            = "nCharNo";
    static const char* const kPartnerCharNo     = "nPartnerCharNo";
    static const char* const kStatus            = "bStatus";
    static const char* const kPromiseCount      = "nPromiseCount";
    static const char* const kMarriageCount     = "nMarriageCount";
    static const char* const kDivorceCancel     = "nDivorceCancel";
    static const char* const kMaxPromiseMember  = "nMAXPROMISEMEMBER";
}
namespace tItem {
    static const char* const kTable        = "tItem";
    static const char* const kItemKey      = "nItemKey";
    static const char* const kOwner        = "nOwner";
    static const char* const kOwnerNo      = "nOwnerNo";
    static const char* const kOwerNo       = "nOwerNo";  // sic, original mis-spelling preserved
    static const char* const kItemID       = "nItemID";
    static const char* const kItemCount    = "nItemCount";
    static const char* const kSlot         = "nSlot";
    static const char* const kSlotNo       = "nSlotNo";
    static const char* const kStorage      = "nStorage";
    static const char* const kStorageType  = "nStorageType";
    static const char* const kPrice        = "nPrice";
    static const char* const kRestCount    = "nRestCount";
    static const char* const kColor        = "nColor";
    static const char* const kIcon         = "nIcon";
    static const char* const kGrade        = "nGrade";
    static const char* const kAllSocketCount     = "nAllSocketCount";
    static const char* const kCreatedSocketCount = "nCreatedSocketCount";
    static const char* const kRestGemCount       = "nRestGemCount";
}
namespace tItemEnchant {
    static const char* const kTable          = "tItemEnchant";
    static const char* const kItemKey        = "nItemKey";
    static const char* const kEquipmentItemKey = "nEquipmentItemKey";
    static const char* const kGemItemKey     = "nGemItemKey";
    static const char* const kGemItemID      = "nGemItemID";
    static const char* const kGemID          = "nGemID";
    static const char* const kGemSocketIndex = "nGemSocketIndex";
}
namespace tItemOptions {
    static const char* const kTable               = "tItemOptions";
    static const char* const kItemKey             = "nItemKey";
    static const char* const kOptionType          = "nOptionType";
    static const char* const kOptionData          = "nOptionData";
    static const char* const kOptionTypeFrom      = "nOptionTypeFrom";
    static const char* const kOptionTypeTo        = "nOptionTypeTo";
    static const char* const kOptionDataFrom      = "nOptionDataFrom";
    static const char* const kOptionDataTo        = "nOptionDataTo";
    static const char* const kOptionDataLotAdd    = "nOptionDataLotAdd";
    static const char* const kOptionDataLotSub    = "nOptionDataLotSub";
    static const char* const kOptionDataLotResult = "nOptionDataLotResult";
}
namespace tMarriage {
    static const char* const kTable      = "tMarriage";
    static const char* const kCharNo     = "nCharNo";
    static const char* const kPartnerCharNo = "nPartnerCharNo";
    static const char* const kStatus     = "bStatus";
}
namespace tPetNew {
    static const char* const kTable      = "tPetNew";
    static const char* const kPetNo      = "nPetNo";
    static const char* const kCharNo     = "nCharNo";
    static const char* const kPetName    = "sPetName";
    static const char* const kPetMind    = "nPetMind";
    static const char* const kPetStress  = "nPetStress";
}
namespace tQuest {
    static const char* const kTable        = "tQuest";
    static const char* const kCharNo       = "nCharNo";
    static const char* const kQuestNo      = "nQuestNo";
    static const char* const kStatus       = "bStatus";
    static const char* const kStatusDone   = "nStatusDone";
    static const char* const kSubStatus    = "nSubStatus";
}
namespace tQuestTimes {
    static const char* const kTable     = "tQuestTimes";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kQuestNo   = "nQuestNo";
    static const char* const kTimes     = "nTimes";
}
namespace tRestExp {
    static const char* const kTable     = "tRestExp";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kExp       = "nExp";
}
namespace tSkill {
    static const char* const kTable               = "tSkill";
    static const char* const kCharNo              = "nCharNo";
    static const char* const kSkillNo             = "nSkillNo";
    static const char* const kSkillPowerSlot      = "nSkillPowerSlot";
    static const char* const kSkillPowerValue     = "nSkillPowerValue";
    static const char* const kSkillPowerDemage    = "nSkillPowerDemage"; // sic, schema spelling
    static const char* const kSkillPowerCoolTime  = "nSkillPowerCoolTime";
    static const char* const kSkillPowerKeepTime  = "nSkillPowerKeepTime";
    static const char* const kSkillPowerSP        = "nSkillPowerSP";
}
namespace tStatCharLevel {
    static const char* const kTable     = "tStatCharLevel";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kLevel     = "nLevel";
    static const char* const kExp       = "nExp";
}
namespace tStatGameMoney {
    static const char* const kTable        = "tStatGameMoney";
    static const char* const kCharNoFrom   = "nCharNoFrom";
    static const char* const kCharNoTo     = "nCharNoTo";
    static const char* const kMoveMoney    = "nMoveMoney";
    static const char* const kTotalBefore  = "nTotalBefore";
    static const char* const kTotalAfter   = "nTotalAfter";
    static const char* const kTaxRate      = "nTaxRate";
}
namespace tUserMoney {
    static const char* const kTable     = "tUserMoney";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kAmount    = "nUserMoney";
}
namespace tGambleCoin {
    static const char* const kTable     = "tGambleCoin";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kCoin      = "nCoin";
}
namespace tGambleCoinExchangeLog {
    static const char* const kTable        = "tGambleCoinExchangeLog";
    static const char* const kCharNo       = "nCharNo";
    static const char* const kBeforeCoin   = "nBeforeCoin";
    static const char* const kAfterCoin    = "nAfterCoin";
    static const char* const kExchangedCoin= "nExchangedCoin";
}
namespace tGambleDice {
    static const char* const kTable     = "tGambleDice";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kDice      = "sDice";
    static const char* const kBatting   = "nBatting";
}
namespace tGambleDiceLog {
    static const char* const kTable     = "tGambleDiceLog";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kBattingMoney = "nBattingMoney";
    static const char* const kIsWin     = "bIsWin";
    static const char* const kDividend  = "nDividend";
}
namespace tGambleJackpot {
    static const char* const kTable        = "tGambleJackpot";
    static const char* const kCharNo       = "nCharNo";
    static const char* const kJackpotCoin  = "nJackpotCoin";
    static const char* const kJackpotRate  = "nJackpotRate";
}
namespace tGambleJackpotLog {
    static const char* const kTable        = "tGambleJackpotLog";
    static const char* const kCharNo       = "nCharNo";
    static const char* const kIsJackpot    = "bIsJackpot";
}
namespace tGambleSlotMachineLog {
    static const char* const kTable     = "tGambleSlotMachineLog";
    static const char* const kCharNo    = "nCharNo";
    static const char* const kBatting   = "nBatting";
    static const char* const kBattingRate = "nBattingRate";
    static const char* const kBattingUnit = "nBattingUnit";
    static const char* const kIsWin     = "bIsWin";
}

// ============================================================================
//  OperatorTool database (logical DB name: OperatorTool)
// ============================================================================
namespace tOperator {
    static const char* const kTable     = "tOperator";
    static const char* const kOperNo    = "nOperNo";
    static const char* const kOperID    = "sOperID";
    static const char* const kOperPW    = "sOperPW";
    static const char* const kOperLevel = "nOperLevel";
}
namespace tMenuAuth {
    static const char* const kTable     = "tMenuAuth";
    static const char* const kOperLevel = "nOperLevel";
    static const char* const kAuthName  = "sAuthName";
}

// ============================================================================
//  StatisticsData database (logical DB name: StatisticsData)
// ============================================================================
namespace tStatAccountLog {
    static const char* const kTable     = "tAccountLog";
    static const char* const kUserNo    = "nUserNo";
    static const char* const kPlayMin   = "nPlayMin";
    static const char* const kDay       = "nDay";
    static const char* const kMon       = "nMon";
}
namespace tSameTimeChar {
    static const char* const kTable     = "tSameTimeChar";
    static const char* const kWorldNo   = "nWorldNo";
    static const char* const kCount     = "nCount";
    static const char* const kAvg       = "nAvg";
    static const char* const kMax       = "nMax";
    static const char* const kMin       = "nMin";
}

// ============================================================================
//  Options database (logical DB name: Options) -- in-row configuration tables
//  used by Options.bak. Keys live in their respective rows; only the singletons
//  the engine consults at boot are listed here.
// ============================================================================
namespace tGameOptionInit {
    static const char* const kTable             = "tGameOptionInit";
    static const char* const kOptionNo          = "nOptionNo";
    static const char* const kDefaultValue      = "nDefaultValue";
    static const char* const kIsActive          = "blsActive";
    static const char* const kDescription       = "sDescription";
    static const char* const kSysCreateDate     = "sys_create_date";
    static const char* const kSysUpdateDate     = "sys_update_date";
}

// ----------------------------------------------------------------------------
//  Back-compat thin accessors. Callers that already use DBField::tUser_nUserNo()
//  continue to work; new code uses the namespaced kXxx constants above.
// ----------------------------------------------------------------------------
} // namespace DB

namespace DBField {
inline const char* tUser_nUserNo()      { return DB::tUser::kUserNo; }
inline const char* tUser_sUserID()      { return DB::tUser::kUserID; }
inline const char* tUser_sUserPW()      { return DB::tUser::kUserPW; }
inline const char* tUser_bIsBlock()     { return DB::tUser::kIsBlock; }
inline const char* tUser_nAuthID()      { return DB::tUser::kAuthID; }
inline const char* tUser_sUserIP()      { return DB::tUser::kUserIP; }
inline const char* tUser_sEmail()       { return DB::tUser::kEmail; }
inline const char* tUserAuth_nAuthID()      { return DB::tUserAuth::kAuthID; }
inline const char* tUserAuth_sAuthName()    { return DB::tUserAuth::kAuthName; }
inline const char* tUserAuth_bIsLoginAble() { return DB::tUserAuth::kIsLoginAble; }
inline const char* tChargeItem_userNo()  { return DB::tChargeItem::kUserNo; }
inline const char* tChargeItem_orderNo() { return DB::tChargeItem::kOrderNo; }
inline const char* tChargeItem_goodsNo() { return DB::tChargeItem::kGoodsNo; }
inline const char* tIPBlock_sIPStart()   { return DB::tIPBlock::kIPStart; }
inline const char* tIPBlock_sIPEnd()     { return DB::tIPBlock::kIPEnd; }
inline const char* tCharacter_nCharNo()      { return DB::tCharacter::kCharNo; }
inline const char* tCharacter_sID()          { return DB::tCharacter::kID; }
inline const char* tCharacter_nUserNo()      { return DB::tCharacter::kUserNo; }
inline const char* tCharacter_nAdminLevel()  { return DB::tCharacter::kAdminLevel; }
inline const char* tCharacter_sLoginZone()   { return DB::tCharacter::kLoginZone; }
inline const char* tCharacter_nLoginZoneX()  { return "nLoginZoneX"; }
inline const char* tCharacter_nLoginZoneY()  { return "nLoginZoneY"; }
inline const char* tCharacter_nLevel()       { return DB::tCharacter::kLevel; }
inline const char* tCharacter_nExp()         { return DB::tCharacter::kExp; }
inline const char* tCharacter_nFame()        { return DB::tCharacter::kFame; }
inline const char* tCharacter_nMoney()       { return DB::tCharacter::kMoney; }
inline const char* tCharacterShape_nCharNo()   { return DB::tCharacterShape::kCharNo; }
inline const char* tCharacterShape_nClass()    { return DB::tCharacterShape::kClass; }
inline const char* tCharacterShape_nGender()   { return "nGender"; }
inline const char* tCharacterShape_nHairType() { return "nHairType"; }
inline const char* tCharacterShape_nHairColor(){ return "nHairColor"; }
inline const char* tCharacterShape_nFaceShape(){ return "nFaceShape"; }
inline const char* tItem_nItemKey() { return DB::tItem::kItemKey; }
inline const char* tItem_nOwner()   { return DB::tItem::kOwner; }
inline const char* tItem_nItemID()  { return DB::tItem::kItemID; }
inline const char* tPrison_nCharID() { return "nCharID"; }
inline const char* tPrison_nMinute() { return "nMinute"; }
inline const char* tPrison_sReason() { return "sReason"; }
inline const char* tGameOptionInit_nOptionNo()      { return DB::tGameOptionInit::kOptionNo; }
inline const char* tGameOptionInit_sys_create_date(){ return DB::tGameOptionInit::kSysCreateDate; }
inline const char* tGameOptionInit_sys_update_date(){ return DB::tGameOptionInit::kSysUpdateDate; }
inline const char* tGameOptionInit_nDefaultValue()  { return DB::tGameOptionInit::kDefaultValue; }
inline const char* tGameOptionInit_blsActive()      { return DB::tGameOptionInit::kIsActive; }
inline const char* tGameOptionInit_sDescription()   { return DB::tGameOptionInit::kDescription; }
inline const char* QuestItemFix_ItemID()  { return "ItemID"; }
inline const char* QuestItemFix_QuestID() { return "QuestID"; }
inline const char* Auto_Account_Creation_Enabled()         { return "Auto_Account_Creation.Enabled"; }
inline const char* Auto_GM_Enabled()                       { return "Auto_GM.Enabled"; }
inline const char* Auto_Master_Enabled()                   { return "Auto_Master.Enabled"; }
inline const char* Auto_Master_nCharNo()                   { return "Auto_Master.nCharNo"; }
inline const char* Character_Rebirth_Enabled()             { return "Character_Rebirth.Enabled"; }
inline const char* Character_Rebirth_Trigger_ItemID()      { return "Character_Rebirth.Trigger_ItemID"; }
inline const char* Character_Rebirth_Trigger_Level()       { return "Character_Rebirth.Trigger_Level"; }
inline const char* Character_Rebirth_Reward_Abstate()      { return "Character_Rebirth.Reward_Abstate"; }
inline const char* Character_Rebirth_Unequip_Character_Items() { return "Character_Rebirth.Unequip_Character_Items"; }
inline const char* Tutorial_Skip_Enabled()                 { return "Tutorial_Skip.Enabled"; }
} // namespace DBField

} // namespace shine
#endif
