// Server/DataServer/Common/SQLP.h
// 03 -- SQLP_<area> stored-procedure facade. Each class corresponds to one
// physical SQL Server database in the original NA2016 deployment:
//
//     Account        -> SQLP_Account
//     AccountLog     -> SQLP_AccountLog
//     World00_Char   -> SQLP_Character + SQLP_Wedding + SQLP_HolyPromise
//                       + SQLP_Guild + SQLP_Item + SQLP_Quest + SQLP_Skill
//                       + SQLP_Friend
//     World00_GameLog-> SQLP_GameLog
//     OperatorTool   -> SQLP_Operator
//     StatisticsData -> SQLP_Statistics
//     Options        -> SQLP_Options
//
// Procedure names are the literal `p_*` / `up_*` identifiers extracted from
// the original *.bak files; do not invent new ones here.
//
// EVIDENCE: PDB_CONFIRMED  symbol class names: SQLP_Account, SQLP_Character,
//           SQLP_Statistics, SQLP_Operator, SQLP_GuildTournament, SQLP_Report,
//           SQLP_IPChecker, SQLP_HolyPromise, SQLP_Wedding.
// EVIDENCE: DATA_CONFIRMED  procedure names: extracted from
//           {Account, AccountLog, World00_Character, OperatorTool,
//            StatisticsData}.bak (2026-02 drop).
#ifndef FIESTA_SQLP_H
#define FIESTA_SQLP_H
#include "Database.h"
#include <string>

namespace fiesta {

class SQLP_Base {
public:
    explicit SQLP_Base(Database* pkDb) : m_pkDb(pkDb) {}
    virtual ~SQLP_Base() {}
    Database* DB() const { return m_pkDb; }
protected:
    Database* m_pkDb;
};

// ----- Account database --------------------------------------------------

// Auth + cash + charge facade. The original dispatcher used `up_Server_*`
// wrapper procs for cross-DB calls; legacy callers still hit those names.
class SQLP_Account : public SQLP_Base {
public:
    SQLP_Account(Database* d) : SQLP_Base(d) {}
    bool   VerifyLogin   (const std::string& rUserID, const std::string& rUserPW, AccountID& aOut, int& iAuthIdOut, bool& bBlockedOut);
    bool   GetAuthList   (uint32 uiAuthIdMin, std::vector<DBRecord>& rOut);     // p_UserGetAuthList
    bool   SetAuthLoginable(uint32 uiAuthId, bool bLoginAble);                  // p_UserSetAuthLoginable
    bool   IsBlocked     (AccountID a);
    bool   GetUserNoByID (const std::string& rUserID, AccountID& aOut);
};

// ----- AccountLog database -----------------------------------------------

class SQLP_AccountLog : public SQLP_Base {
public:
    SQLP_AccountLog(Database* d) : SQLP_Base(d) {}
    bool   IPBlockIsBlock(const std::string& rIP);                              // p_IPBlock_IsBlock
    bool   EventFriendGet(AccountID a, std::vector<DBRecord>& rOut);            // p_Event_Friend_Get
    void   AppendLogin   (AccountID a, uint8 uiWorldNo);
    void   AppendLogout  (AccountID a, uint8 uiWorldNo);
};

// ----- World00_Character database ----------------------------------------

class SQLP_Character : public SQLP_Base {
public:
    SQLP_Character(Database* d) : SQLP_Base(d) {}
    bool   Login        (CharID c, std::vector<DBRecord>& rOut);                // p_Char_Login
    bool   Logout       (CharID c);                                             // p_Char_Logout
    bool   MoneyMove    (CharID cFrom, CharID cTo, uint64 uiAmount, uint16 uiTaxRate); // p_Char_MoneyMove
    bool   MoneySub     (CharID c, uint64 uiAmount);                            // p_Char_MoneySub
    bool   FameSub      (CharID c, uint32 uiAmount);                            // p_Char_FameSub
};

class SQLP_Wedding : public SQLP_Base {
public:
    SQLP_Wedding(Database* d) : SQLP_Base(d) {}
    bool   StatusGet      (CharID c, DBRecord& rOut);                           // p_Char_WeddingStatus_Get
    bool   StatusSet      (CharID c, int iStatus);                              // p_Char_WeddingStatus_Set
    bool   Propose        (CharID cFrom, CharID cTo);                           // p_Char_Wedding_Propose
    bool   CancelPropose  (CharID cFrom, CharID cTo);                           // p_Char_Wedding_Cancel_Propose
    bool   Do             (CharID cA, CharID cB);                               // p_Char_Wedding_Do
    bool   DivorceCancel  (CharID c);                                           // p_Char_Wedding_Divorce_Cancel
    bool   DivorceDo      (CharID c);                                           // p_Char_Wedding_Divorce_Do
    bool   DivorceByAgree (CharID c);                                           // p_Char_Wedding_Divorce_by_Agree
    bool   DivorceByForce (CharID c);                                           // p_Char_Wedding_Divorce_by_Force
    bool   EscapeDivorceState(CharID c);                                        // p_Char_Wedding_Escape_Divorce_State
    bool   GetPartnerInfo (CharID c, DBRecord& rOut);                           // p_Char_Wedding_Get_Partner_Info
};

class SQLP_HolyPromise : public SQLP_Base {
public:
    SQLP_HolyPromise(Database* d) : SQLP_Base(d) {}
    bool   CheckRing       (CharID c, uint32 uiItemID);                         // p_HolyPromise_CheckRing
    bool   Get             (CharID c, DBRecord& rOut);                          // p_HolyPromise_Get
    bool   Set             (CharID cA, CharID cB);                              // p_HolyPromise_Set
    bool   SetDate         (CharID c);                                          // p_HolyPromise_SetDate
    bool   Del             (CharID c);                                          // p_HolyPromise_Del
    bool   DelChar         (CharID c);                                          // p_HolyPromise_DelChar
    bool   GetMember       (CharID c, std::vector<DBRecord>& rOut);             // p_HolyPromise_GetMember
    bool   GetMemberCount  (CharID c, uint32& uiOut);                           // p_HolyPromise_GetMemberCount
};

class SQLP_Guild : public SQLP_Base {
public:
    SQLP_Guild(Database* d) : SQLP_Base(d) {}
    bool   GetListDetail              (std::vector<DBRecord>& rOut);            // p_Guild_GetListDetail
    bool   TournamentGet              (uint32 uiGTNo, DBRecord& rOut);          // p_GuildTournament_Get
    bool   TournamentSet              (uint32 uiGTNo, uint32 uiGuildNo, int iStatus); // p_GuildTournament_Set
    bool   TournamentGetAllList       (std::vector<DBRecord>& rOut);            // p_GuildTournament_GetAllTournamentList
    bool   TournamentGetLastMatch     (DBRecord& rOut);                         // p_GuildTournament_GetLastMatch
};

class SQLP_Item : public SQLP_Base {
public:
    SQLP_Item(Database* d) : SQLP_Base(d) {}
    bool   Create          (CharID cOwner, uint32 uiItemID, uint16 uiCount,
                            uint8  uiStorage, uint16 uiSlot, uint64& uiItemKeyOut); // p_Item_Create
    bool   Delete          (uint64 uiItemKey);                                  // p_Item_Delete
    bool   BuyAll          (CharID cOwner, uint32 uiItemID, uint16 uiCount, uint64 uiPrice); // p_Item_BuyAll
    bool   BuyLot          (CharID cOwner, uint32 uiItemID, uint16 uiCount, uint64 uiPrice); // p_Item_BuyLot
    bool   GetOption       (uint64 uiItemKey, std::vector<DBRecord>& rOut);     // p_Item_GetOption
    bool   SetOption       (uint64 uiItemKey, uint8 uiType, int32 iData);       // p_Item_SetOption
    bool   IsValidOwner    (uint64 uiItemKey, CharID cOwner);                   // p_Item_IsValidOwner
    bool   GetStorageTypeCount(CharID cOwner, uint8 uiStorageType, uint32& uiOut); // p_Item_GetStorageTypeCount
    bool   MergeNew        (uint64 uiSrcItemKey, uint64 uiDstItemKey, uint16 uiSrcCount); // p_Item_Merge_New
    bool   SplitNMergeNew  (uint64 uiSrcItemKey, uint16 uiSplitCount, uint64 uiDstItemKey); // p_Item_Split_N_Merge_New
};

class SQLP_Quest : public SQLP_Base {
public:
    SQLP_Quest(Database* d) : SQLP_Base(d) {}
    bool   GetAllDoing  (CharID c, std::vector<DBRecord>& rOut);                // p_Quest_GetAllDoing
    bool   Set          (CharID c, uint32 uiQuestNo, int iStatus, int iSubStatus); // p_Quest_Set
    bool   GetTimes     (CharID c, uint32 uiQuestNo, uint32& uiOut);            // p_getQuestTimes
};

class SQLP_Skill : public SQLP_Base {
public:
    SQLP_Skill(Database* d) : SQLP_Base(d) {}
    bool   SetPower     (CharID c, uint32 uiSkillNo, int iSlot, int iValue,
                         int iDamage, int iCool, int iKeep, int iSP);           // p_Skill_SetPower
    bool   SetPowerAll  (CharID c);                                             // p_Skill_SetPowerAll
};

class SQLP_Friend : public SQLP_Base {
public:
    SQLP_Friend(Database* d) : SQLP_Base(d) {}
    bool   DelAll       (CharID c);                                             // p_Friend_Del_All
};

// Estate / mini-house persistence. Layout matches the in-memory `EstateRec`
// in Server/Zone/EstateSystem.h:
//   tEstate           (CharID PK, uiHouseId, uiTier, uiEndure, uiNextDecay)
//   tEstateFurniture  (CharID, slot PK, furnId, X, Y, Z, yawDeg, endure)
class SQLP_Estate : public SQLP_Base {
public:
    SQLP_Estate(Database* d) : SQLP_Base(d) {}
    bool   Create   (CharID owner, uint32 uiHouseId, uint32 uiTier);            // p_Estate_Create
    bool   Demolish (CharID owner);                                             // p_Estate_Demolish
    bool   Save     (CharID owner, const uint8* pData, size_t uiLen);           // p_Estate_SaveFurniture
    bool   Load     (CharID owner, std::vector<DBRecord>& rOut);                // p_Estate_LoadFurniture
};

// ----- World00_GameLog database ------------------------------------------

class SQLP_GameLog : public SQLP_Base {
public:
    SQLP_GameLog(Database* d) : SQLP_Base(d) {}
    void   LogKill      (CharID cKiller, MobID mob, uint16 uiMapID);
    void   LogDrop      (CharID cOwner, uint32 uiItemID, uint16 uiCount);
    void   LogTrade     (CharID cFrom, CharID cTo, uint64 uiItemKey, uint16 uiCount);
};

// ----- StatisticsData database -------------------------------------------

class SQLP_Statistics : public SQLP_Base {
public:
    SQLP_Statistics(Database* d) : SQLP_Base(d) {}
    void   OnCharLogin       (CharID c);
    void   OnCharLogout      (CharID c);
    bool   DayConnectionTime (AccountID a, uint32 uiPlayMin);                   // p_DayConnectionTime
    void   RecordSameTimeChar(uint8 uiWorldNo, uint32 uiCount);
};

// ----- OperatorTool database ---------------------------------------------

class SQLP_Operator : public SQLP_Base {
public:
    SQLP_Operator(Database* d) : SQLP_Base(d) {}
    bool   Logon       (const std::string& rOperID, const std::string& rOperPW, int& iLevelOut); // p_Operator_Logon
    bool   Del         (const std::string& rOperID);                            // p_Operator_Del
    bool   SetLevel    (const std::string& rOperID, int iLevel);                // p_Operator_SetLevel
    bool   SetPW       (const std::string& rOperID, const std::string& rOldPW, const std::string& rNewPW); // p_Operator_SetPW
};

// ----- Options database --------------------------------------------------

class SQLP_Options : public SQLP_Base {
public:
    SQLP_Options(Database* d) : SQLP_Base(d) {}
    bool   GetInt    (const std::string& rKey, int32& iOut);
    bool   GetString (const std::string& rKey, std::string& rOut);
};

// ----- Generic report / ip-checker (used cross-DB) -----------------------

class SQLP_Report    : public SQLP_Base { public: SQLP_Report   (Database* d):SQLP_Base(d){} void File(const std::string& rKind,const std::string& rPayload); };
class SQLP_IPChecker : public SQLP_Base { public: SQLP_IPChecker(Database* d):SQLP_Base(d){} bool IsBlocked(const std::string& rIP); };

} // namespace fiesta
#endif
