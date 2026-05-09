// Server/DataServer/Common/DBSchemaConstants.h
// EVIDENCE: DATA_CONFIRMED  source: database_doc_fields.csv
// Fully-qualified column names from the original schema documentation.
// Use these constants in SQL string composition to avoid typos.
#ifndef FIESTA_DBSCHEMA_CONST_H
#define FIESTA_DBSCHEMA_CONST_H
namespace fiesta { namespace DBField {
// -- Account Database --
inline const char* tUser_nUserNo() { return "tUser.nUserNo"; }
inline const char* tUser_sUserID() { return "tUser.sUserID"; }
inline const char* tUser_sUserPW() { return "tUser.sUserPW"; }
inline const char* tUser_bIsBlock() { return "tUser.bIsBlock"; }
inline const char* tUser_nAuthID() { return "tUser.nAuthID"; }
inline const char* tUser_sUserIP() { return "tUser.sUserIP"; }
inline const char* tUser_sEmail() { return "tUser.sEmail"; }
inline const char* tUserAuth_nAuthID() { return "tUserAuth.nAuthID"; }
inline const char* tUserAuth_sAuthName() { return "tUserAuth.sAuthName"; }
inline const char* tUserAuth_bIsLoginAble() { return "tUserAuth.bIsLoginAble"; }
inline const char* tChargeItem_userNo() { return "tChargeItem.userNo"; }
inline const char* tChargeItem_orderNo() { return "tChargeItem.orderNo"; }
inline const char* tChargeItem_goodsNo() { return "tChargeItem.goodsNo"; }
// -- AccountLog Database --
inline const char* tIPBlock_sIPStart() { return "tIPBlock.sIPStart"; }
inline const char* tIPBlock_sIPEnd() { return "tIPBlock.sIPEnd"; }
// -- World00_Character Database --
inline const char* tCharacter_nCharNo() { return "tCharacter.nCharNo"; }
inline const char* tCharacter_sID() { return "tCharacter.sID"; }
inline const char* tCharacter_nUserNo() { return "tCharacter.nUserNo"; }
inline const char* tCharacter_nAdminLevel() { return "tCharacter.nAdminLevel"; }
inline const char* tCharacter_sLoginZone() { return "tCharacter.sLoginZone"; }
inline const char* tCharacter_nLoginZoneX() { return "tCharacter.nLoginZoneX"; }
inline const char* tCharacter_nLoginZoneY() { return "tCharacter.nLoginZoneY"; }
inline const char* tCharacter_nLevel() { return "tCharacter.nLevel"; }
inline const char* tCharacter_nExp() { return "tCharacter.nExp"; }
inline const char* tCharacter_nFame() { return "tCharacter.nFame"; }
inline const char* tCharacter_nMoney() { return "tCharacter.nMoney"; }
inline const char* QuestItemFix_ItemID() { return "QuestItemFix.ItemID"; }
inline const char* QuestItemFix_QuestID() { return "QuestItemFix.QuestID"; }
inline const char* tCharacterShape_nCharNo() { return "tCharacterShape.nCharNo"; }
inline const char* tCharacterShape_nClass() { return "tCharacterShape.nClass"; }
inline const char* tCharacterShape_nGender() { return "tCharacterShape.nGender"; }
inline const char* tCharacterShape_nHairType() { return "tCharacterShape.nHairType"; }
inline const char* tCharacterShape_nHairColor() { return "tCharacterShape.nHairColor"; }
inline const char* tCharacterShape_nFaceShape() { return "tCharacterShape.nFaceShape"; }
inline const char* tItem_nItemKey() { return "tItem.nItemKey"; }
inline const char* tItem_nOwner() { return "tItem.nOwner"; }
inline const char* tItem_nItemID() { return "tItem.nItemID"; }
inline const char* tPrison_nCharID() { return "tPrison.nCharID"; }
inline const char* tPrison_nMinute() { return "tPrison.nMinute"; }
inline const char* tPrison_sReason() { return "tPrison.sReason"; }
inline const char* tGameOptionInit_nOptionNo() { return "tGameOptionInit.nOptionNo"; }
inline const char* tGameOptionInit_sys_create_date() { return "tGameOptionInit.sys_create_date"; }
inline const char* tGameOptionInit_sys_update_date() { return "tGameOptionInit.sys_update_date"; }
inline const char* tGameOptionInit_nDefaultValue() { return "tGameOptionInit.nDefaultValue"; }
inline const char* tGameOptionInit_blsActive() { return "tGameOptionInit.blsActive"; }
inline const char* tGameOptionInit_sDescription() { return "tGameOptionInit.sDescription"; }
// -- Options Database --
inline const char* Auto_Account_Creation_Enabled() { return "Auto_Account_Creation.Enabled"; }
inline const char* Auto_GM_Enabled() { return "Auto_GM.Enabled"; }
inline const char* Auto_Master_Enabled() { return "Auto_Master.Enabled"; }
inline const char* Auto_Master_nCharNo() { return "Auto_Master.nCharNo"; }
inline const char* Character_Rebirth_Enabled() { return "Character_Rebirth.Enabled"; }
inline const char* Character_Rebirth_Trigger_ItemID() { return "Character_Rebirth.Trigger_ItemID"; }
inline const char* Character_Rebirth_Trigger_Level() { return "Character_Rebirth.Trigger_Level"; }
inline const char* Character_Rebirth_Reward_Abstate() { return "Character_Rebirth.Reward_Abstate"; }
inline const char* Character_Rebirth_Unequip_Character_Items() { return "Character_Rebirth.Unequip_Character_Items"; }
inline const char* Tutorial_Skip_Enabled() { return "Tutorial_Skip.Enabled"; }
} } // namespace fiesta::DBField
#endif