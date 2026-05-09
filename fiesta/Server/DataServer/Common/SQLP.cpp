// Server/DataServer/Common/SQLP.cpp
// Implementations for every SQLP_* facade.  All proc names are the literal
// `p_*` identifiers extracted from the original .bak files; see SQLP.h banner
// for evidence trail.  Argument shapes follow the documented stored-procedure
// parameter lists; mismatches will surface at first proc call against a real
// schema and should be corrected here only.
//
// Concurrency: every call funnels through `Database::Exec/Query`, both of
// which take the per-connection critical section, so each SQLP_* instance is
// thread-safe at single-statement granularity.
#include "SQLP.h"
#include "DBSchemaConstants.h"
#include "../../Shared/ShineLogSystem.h"
#include <stdio.h>

namespace fiesta {

// ---------------------------------------------------------------------------
//  Internal helpers (file-local).
// ---------------------------------------------------------------------------
namespace {
    // sprintf-style string builder bounded to 256 bytes (proc-arg buffers).
    inline std::string F(const char* fmt, ...) {
        char buf[256];
        va_list ap; va_start(ap, fmt);
        vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);
        buf[sizeof(buf)-1] = '\0';
        return std::string(buf);
    }
    inline std::string Quoted(const std::string& s) {
        std::string out("'"); out += Database::Quote(s); out += "'"; return out;
    }
    // pull a uint32 out of column 0 if a result set arrived
    inline uint32 Col0U32(const std::vector<DBRecord>& rows, uint32 fallback = 0) {
        return rows.empty() ? fallback : rows[0].GetU32(0);
    }
    inline int Col0I32(const std::vector<DBRecord>& rows, int fallback = 0) {
        return rows.empty() ? fallback : rows[0].GetInt(0);
    }
}

// ===========================================================================
//  SQLP_Account     (DB: Account)
// ===========================================================================

bool SQLP_Account::VerifyLogin(const std::string& rUserID, const std::string& rUserPW,
                               AccountID& aOut, int& iAuthIdOut, bool& bBlockedOut)
{
    aOut = 0; iAuthIdOut = 0; bBlockedOut = false;
    if (!m_pkDb || !m_pkDb->IsConnected()) return false;
    // Documented wrapper proc that performs sUserID/sUserPW match against tUser
    // and returns nUserNo, nAuthID, bIsBlock.  Falls back to inline SELECT when
    // the wrapper is not deployed (private servers occasionally drop it).
    std::vector<DBRecord> kRows;
    std::string kArgs = Quoted(rUserID); kArgs += ","; kArgs += Quoted(rUserPW);
    if (!m_pkDb->QueryProc("up_Server_Account_Login", kArgs, kRows) || kRows.empty()) {
        std::string kSql = "SELECT ";
        kSql += DB::tUser::kUserNo; kSql += ","; kSql += DB::tUser::kAuthID;
        kSql += ","; kSql += DB::tUser::kIsBlock;
        kSql += " FROM "; kSql += DB::tUser::kTable;
        kSql += " WHERE "; kSql += DB::tUser::kUserID; kSql += "="; kSql += Quoted(rUserID);
        kSql += " AND ";   kSql += DB::tUser::kUserPW; kSql += "="; kSql += Quoted(rUserPW);
        if (!m_pkDb->Query(kSql, kRows) || kRows.empty()) return false;
    }
    aOut        = kRows[0].GetU32(0);
    iAuthIdOut  = kRows[0].GetInt(1);
    bBlockedOut = (kRows[0].GetInt(2) != 0);
    return aOut != 0;
}

bool SQLP_Account::GetAuthList(uint32 uiAuthIdMin, std::vector<DBRecord>& rOut) {
    if (!m_pkDb) return false;
    return m_pkDb->QueryProc("p_UserGetAuthList", F("%u", uiAuthIdMin), rOut);
}

bool SQLP_Account::SetAuthLoginable(uint32 uiAuthId, bool bLoginAble) {
    if (!m_pkDb) return false;
    return m_pkDb->ExecProc("p_UserSetAuthLoginable", F("%u,%d", uiAuthId, bLoginAble?1:0));
}

bool SQLP_Account::IsBlocked(AccountID a) {
    if (!m_pkDb) return false;
    std::string kSql = "SELECT "; kSql += DB::tUser::kIsBlock;
    kSql += " FROM "; kSql += DB::tUser::kTable;
    kSql += " WHERE "; kSql += DB::tUser::kUserNo; kSql += "="; kSql += F("%u", a);
    std::vector<DBRecord> rows;
    return m_pkDb->Query(kSql, rows) && Col0I32(rows) != 0;
}

bool SQLP_Account::GetUserNoByID(const std::string& rUserID, AccountID& aOut) {
    if (!m_pkDb) return false;
    std::string kSql = "SELECT "; kSql += DB::tUser::kUserNo;
    kSql += " FROM "; kSql += DB::tUser::kTable;
    kSql += " WHERE "; kSql += DB::tUser::kUserID; kSql += "="; kSql += Quoted(rUserID);
    std::vector<DBRecord> rows;
    if (!m_pkDb->Query(kSql, rows) || rows.empty()) { aOut = 0; return false; }
    aOut = rows[0].GetU32(0);
    return aOut != 0;
}

// ===========================================================================
//  SQLP_AccountLog  (DB: AccountLog)
// ===========================================================================

bool SQLP_AccountLog::IPBlockIsBlock(const std::string& rIP) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_IPBlock_IsBlock", Quoted(rIP), rows)) return false;
    return Col0I32(rows) != 0;
}

bool SQLP_AccountLog::EventFriendGet(AccountID a, std::vector<DBRecord>& rOut) {
    if (!m_pkDb) return false;
    return m_pkDb->QueryProc("p_Event_Friend_Get", F("%u", a), rOut);
}

void SQLP_AccountLog::AppendLogin(AccountID a, uint8 uiWorldNo) {
    if (!m_pkDb) return;
    std::string kSql = "INSERT INTO "; kSql += DB::tAccountLog::kTable;
    kSql += " ("; kSql += DB::tAccountLog::kUserNo; kSql += ",";
    kSql += DB::tAccountLog::kWorldNo; kSql += ","; kSql += DB::tAccountLog::kType;
    kSql += ") VALUES ("; kSql += F("%u,%u,1", a, (uint32)uiWorldNo); kSql += ")";
    m_pkDb->Exec(kSql);
}
void SQLP_AccountLog::AppendLogout(AccountID a, uint8 uiWorldNo) {
    if (!m_pkDb) return;
    std::string kSql = "INSERT INTO "; kSql += DB::tAccountLog::kTable;
    kSql += " ("; kSql += DB::tAccountLog::kUserNo; kSql += ",";
    kSql += DB::tAccountLog::kWorldNo; kSql += ","; kSql += DB::tAccountLog::kType;
    kSql += ") VALUES ("; kSql += F("%u,%u,2", a, (uint32)uiWorldNo); kSql += ")";
    m_pkDb->Exec(kSql);
}

// ===========================================================================
//  SQLP_Character   (DB: World00_Character)
// ===========================================================================

bool SQLP_Character::Login(CharID c, std::vector<DBRecord>& rOut) {
    return m_pkDb && m_pkDb->QueryProc("p_Char_Login", F("%u", c), rOut);
}
bool SQLP_Character::Logout(CharID c) {
    return m_pkDb && m_pkDb->ExecProc("p_Char_Logout", F("%u", c));
}
bool SQLP_Character::MoneyMove(CharID cFrom, CharID cTo, uint64 uiAmount, uint16 uiTaxRate) {
    return m_pkDb && m_pkDb->ExecProc("p_Char_MoneyMove",
        F("%u,%u,%llu,%u", cFrom, cTo, (unsigned long long)uiAmount, (uint32)uiTaxRate));
}
bool SQLP_Character::MoneySub(CharID c, uint64 uiAmount) {
    return m_pkDb && m_pkDb->ExecProc("p_Char_MoneySub",
        F("%u,%llu", c, (unsigned long long)uiAmount));
}
bool SQLP_Character::FameSub(CharID c, uint32 uiAmount) {
    return m_pkDb && m_pkDb->ExecProc("p_Char_FameSub", F("%u,%u", c, uiAmount));
}

// ===========================================================================
//  SQLP_Wedding     (DB: World00_Character)
// ===========================================================================

bool SQLP_Wedding::StatusGet(CharID c, DBRecord& rOut) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_Char_WeddingStatus_Get", F("%u", c), rows) || rows.empty()) return false;
    rOut = rows[0]; return true;
}
bool SQLP_Wedding::StatusSet(CharID c, int iStatus) {
    return m_pkDb && m_pkDb->ExecProc("p_Char_WeddingStatus_Set", F("%u,%d", c, iStatus));
}
bool SQLP_Wedding::Propose(CharID a, CharID b)        { return m_pkDb && m_pkDb->ExecProc("p_Char_Wedding_Propose",            F("%u,%u", a, b)); }
bool SQLP_Wedding::CancelPropose(CharID a, CharID b)  { return m_pkDb && m_pkDb->ExecProc("p_Char_Wedding_Cancel_Propose",     F("%u,%u", a, b)); }
bool SQLP_Wedding::Do(CharID a, CharID b)             { return m_pkDb && m_pkDb->ExecProc("p_Char_Wedding_Do",                 F("%u,%u", a, b)); }
bool SQLP_Wedding::DivorceCancel(CharID c)            { return m_pkDb && m_pkDb->ExecProc("p_Char_Wedding_Divorce_Cancel",     F("%u",   c));    }
bool SQLP_Wedding::DivorceDo(CharID c)                { return m_pkDb && m_pkDb->ExecProc("p_Char_Wedding_Divorce_Do",         F("%u",   c));    }
bool SQLP_Wedding::DivorceByAgree(CharID c)           { return m_pkDb && m_pkDb->ExecProc("p_Char_Wedding_Divorce_by_Agree",   F("%u",   c));    }
bool SQLP_Wedding::DivorceByForce(CharID c)           { return m_pkDb && m_pkDb->ExecProc("p_Char_Wedding_Divorce_by_Force",   F("%u",   c));    }
bool SQLP_Wedding::EscapeDivorceState(CharID c)       { return m_pkDb && m_pkDb->ExecProc("p_Char_Wedding_Escape_Divorce_State", F("%u", c));    }
bool SQLP_Wedding::GetPartnerInfo(CharID c, DBRecord& rOut) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_Char_Wedding_Get_Partner_Info", F("%u", c), rows) || rows.empty()) return false;
    rOut = rows[0]; return true;
}

// ===========================================================================
//  SQLP_HolyPromise  (DB: World00_Character)
// ===========================================================================

bool SQLP_HolyPromise::CheckRing(CharID c, uint32 uiItemID) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_HolyPromise_CheckRing", F("%u,%u", c, uiItemID), rows)) return false;
    return Col0I32(rows) != 0;
}
bool SQLP_HolyPromise::Get(CharID c, DBRecord& rOut) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_HolyPromise_Get", F("%u", c), rows) || rows.empty()) return false;
    rOut = rows[0]; return true;
}
bool SQLP_HolyPromise::Set(CharID a, CharID b)    { return m_pkDb && m_pkDb->ExecProc("p_HolyPromise_Set",     F("%u,%u", a, b)); }
bool SQLP_HolyPromise::SetDate(CharID c)          { return m_pkDb && m_pkDb->ExecProc("p_HolyPromise_SetDate", F("%u",    c));    }
bool SQLP_HolyPromise::Del(CharID c)              { return m_pkDb && m_pkDb->ExecProc("p_HolyPromise_Del",     F("%u",    c));    }
bool SQLP_HolyPromise::DelChar(CharID c)          { return m_pkDb && m_pkDb->ExecProc("p_HolyPromise_DelChar", F("%u",    c));    }
bool SQLP_HolyPromise::GetMember(CharID c, std::vector<DBRecord>& rOut) {
    return m_pkDb && m_pkDb->QueryProc("p_HolyPromise_GetMember", F("%u", c), rOut);
}
bool SQLP_HolyPromise::GetMemberCount(CharID c, uint32& uiOut) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_HolyPromise_GetMemberCount", F("%u", c), rows) || rows.empty()) { uiOut = 0; return false; }
    uiOut = rows[0].GetU32(0); return true;
}

// ===========================================================================
//  SQLP_Guild       (DB: World00_Character)
// ===========================================================================

bool SQLP_Guild::GetListDetail(std::vector<DBRecord>& rOut) {
    return m_pkDb && m_pkDb->QueryProc("p_Guild_GetListDetail", "", rOut);
}
bool SQLP_Guild::TournamentGet(uint32 uiGTNo, DBRecord& rOut) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_GuildTournament_Get", F("%u", uiGTNo), rows) || rows.empty()) return false;
    rOut = rows[0]; return true;
}
bool SQLP_Guild::TournamentSet(uint32 uiGTNo, uint32 uiGuildNo, int iStatus) {
    return m_pkDb && m_pkDb->ExecProc("p_GuildTournament_Set", F("%u,%u,%d", uiGTNo, uiGuildNo, iStatus));
}
bool SQLP_Guild::TournamentGetAllList(std::vector<DBRecord>& rOut) {
    return m_pkDb && m_pkDb->QueryProc("p_GuildTournament_GetAllTournamentList", "", rOut);
}
bool SQLP_Guild::TournamentGetLastMatch(DBRecord& rOut) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_GuildTournament_GetLastMatch", "", rows) || rows.empty()) return false;
    rOut = rows[0]; return true;
}

// ===========================================================================
//  SQLP_Item        (DB: World00_Character)
// ===========================================================================

bool SQLP_Item::Create(CharID cOwner, uint32 uiItemID, uint16 uiCount,
                       uint8 uiStorage, uint16 uiSlot, uint64& uiItemKeyOut)
{
    uiItemKeyOut = 0;
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    bool ok = m_pkDb->QueryProc("p_Item_Create",
        F("%u,%u,%u,%u,%u", cOwner, uiItemID, (uint32)uiCount, (uint32)uiStorage, (uint32)uiSlot),
        rows);
    if (!ok || rows.empty()) return false;
    uiItemKeyOut = (uint64)rows[0].GetU32(0);
    return uiItemKeyOut != 0;
}
bool SQLP_Item::Delete(uint64 uiItemKey) {
    return m_pkDb && m_pkDb->ExecProc("p_Item_Delete", F("%llu", (unsigned long long)uiItemKey));
}
bool SQLP_Item::BuyAll(CharID cOwner, uint32 uiItemID, uint16 uiCount, uint64 uiPrice) {
    return m_pkDb && m_pkDb->ExecProc("p_Item_BuyAll",
        F("%u,%u,%u,%llu", cOwner, uiItemID, (uint32)uiCount, (unsigned long long)uiPrice));
}
bool SQLP_Item::BuyLot(CharID cOwner, uint32 uiItemID, uint16 uiCount, uint64 uiPrice) {
    return m_pkDb && m_pkDb->ExecProc("p_Item_BuyLot",
        F("%u,%u,%u,%llu", cOwner, uiItemID, (uint32)uiCount, (unsigned long long)uiPrice));
}
bool SQLP_Item::GetOption(uint64 uiItemKey, std::vector<DBRecord>& rOut) {
    return m_pkDb && m_pkDb->QueryProc("p_Item_GetOption", F("%llu", (unsigned long long)uiItemKey), rOut);
}
bool SQLP_Item::SetOption(uint64 uiItemKey, uint8 uiType, int32 iData) {
    return m_pkDb && m_pkDb->ExecProc("p_Item_SetOption",
        F("%llu,%u,%d", (unsigned long long)uiItemKey, (uint32)uiType, iData));
}
bool SQLP_Item::IsValidOwner(uint64 uiItemKey, CharID cOwner) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_Item_IsValidOwner",
        F("%llu,%u", (unsigned long long)uiItemKey, cOwner), rows)) return false;
    return Col0I32(rows) != 0;
}
bool SQLP_Item::GetStorageTypeCount(CharID cOwner, uint8 uiStorageType, uint32& uiOut) {
    uiOut = 0;
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_Item_GetStorageTypeCount",
        F("%u,%u", cOwner, (uint32)uiStorageType), rows) || rows.empty()) return false;
    uiOut = rows[0].GetU32(0); return true;
}
bool SQLP_Item::MergeNew(uint64 uiSrc, uint64 uiDst, uint16 uiSrcCount) {
    return m_pkDb && m_pkDb->ExecProc("p_Item_Merge_New",
        F("%llu,%llu,%u", (unsigned long long)uiSrc, (unsigned long long)uiDst, (uint32)uiSrcCount));
}
bool SQLP_Item::SplitNMergeNew(uint64 uiSrc, uint16 uiSplit, uint64 uiDst) {
    return m_pkDb && m_pkDb->ExecProc("p_Item_Split_N_Merge_New",
        F("%llu,%u,%llu", (unsigned long long)uiSrc, (uint32)uiSplit, (unsigned long long)uiDst));
}

// ===========================================================================
//  SQLP_Quest / SQLP_Skill / SQLP_Friend
// ===========================================================================

bool SQLP_Quest::GetAllDoing(CharID c, std::vector<DBRecord>& rOut) {
    return m_pkDb && m_pkDb->QueryProc("p_Quest_GetAllDoing", F("%u", c), rOut);
}
bool SQLP_Quest::Set(CharID c, uint32 uiQuestNo, int iStatus, int iSubStatus) {
    return m_pkDb && m_pkDb->ExecProc("p_Quest_Set",
        F("%u,%u,%d,%d", c, uiQuestNo, iStatus, iSubStatus));
}
bool SQLP_Quest::GetTimes(CharID c, uint32 uiQuestNo, uint32& uiOut) {
    uiOut = 0;
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("p_getQuestTimes", F("%u,%u", c, uiQuestNo), rows) || rows.empty()) return false;
    uiOut = rows[0].GetU32(0); return true;
}

bool SQLP_Skill::SetPower(CharID c, uint32 uiSkillNo, int iSlot, int iValue,
                          int iDamage, int iCool, int iKeep, int iSP)
{
    return m_pkDb && m_pkDb->ExecProc("p_Skill_SetPower",
        F("%u,%u,%d,%d,%d,%d,%d,%d", c, uiSkillNo, iSlot, iValue, iDamage, iCool, iKeep, iSP));
}
bool SQLP_Skill::SetPowerAll(CharID c) {
    return m_pkDb && m_pkDb->ExecProc("p_Skill_SetPowerAll", F("%u", c));
}

bool SQLP_Friend::DelAll(CharID c) {
    return m_pkDb && m_pkDb->ExecProc("p_Friend_Del_All", F("%u", c));
}

// ===========================================================================
//  SQLP_GameLog     (DB: World00_GameLog) -- append-only writes
// ===========================================================================

void SQLP_GameLog::LogKill(CharID c, MobID m, uint16 uiMapID) {
    if (!m_pkDb) return;
    char sql[256];
    sprintf_s(sql, sizeof(sql),
        "INSERT INTO tMobHuntLog(nCharNo,nMobID,nMapID) VALUES(%u,%u,%u)",
        c, m, (uint32)uiMapID);
    m_pkDb->Exec(sql);
}
void SQLP_GameLog::LogDrop(CharID c, uint32 uiItemID, uint16 uiCount) {
    if (!m_pkDb) return;
    char sql[256];
    sprintf_s(sql, sizeof(sql),
        "INSERT INTO tItemDropLog(nCharNo,nItemID,nCount) VALUES(%u,%u,%u)",
        c, uiItemID, (uint32)uiCount);
    m_pkDb->Exec(sql);
}
void SQLP_GameLog::LogTrade(CharID a, CharID b, uint64 uiItemKey, uint16 uiCount) {
    if (!m_pkDb) return;
    char sql[256];
    sprintf_s(sql, sizeof(sql),
        "INSERT INTO tTradeLog(nCharNoFrom,nCharNoTo,nItemKey,nCount) VALUES(%u,%u,%llu,%u)",
        a, b, (unsigned long long)uiItemKey, (uint32)uiCount);
    m_pkDb->Exec(sql);
}

// ===========================================================================
//  SQLP_Statistics  (DB: StatisticsData)
// ===========================================================================

void SQLP_Statistics::OnCharLogin(CharID c) {
    if (!m_pkDb) return;
    m_pkDb->ExecProc("up_Server_Statistics_CharLogin", F("%u", c));
}
void SQLP_Statistics::OnCharLogout(CharID c) {
    if (!m_pkDb) return;
    m_pkDb->ExecProc("up_Server_Statistics_CharLogout", F("%u", c));
}
bool SQLP_Statistics::DayConnectionTime(AccountID a, uint32 uiPlayMin) {
    return m_pkDb && m_pkDb->ExecProc("p_DayConnectionTime", F("%u,%u", a, uiPlayMin));
}
void SQLP_Statistics::RecordSameTimeChar(uint8 uiWorldNo, uint32 uiCount) {
    if (!m_pkDb) return;
    char sql[256];
    sprintf_s(sql, sizeof(sql),
        "INSERT INTO tSameTimeChar(nWorldNo,nCount) VALUES(%u,%u)",
        (uint32)uiWorldNo, uiCount);
    m_pkDb->Exec(sql);
}

// ===========================================================================
//  SQLP_Operator    (DB: OperatorTool)
// ===========================================================================

bool SQLP_Operator::Logon(const std::string& rOperID, const std::string& rOperPW, int& iLevelOut) {
    iLevelOut = 0;
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    std::string args = Quoted(rOperID); args += ","; args += Quoted(rOperPW);
    if (!m_pkDb->QueryProc("p_Operator_Logon", args, rows) || rows.empty()) return false;
    iLevelOut = rows[0].GetInt(0);
    return iLevelOut >= 0;
}
bool SQLP_Operator::Del(const std::string& rOperID) {
    return m_pkDb && m_pkDb->ExecProc("p_Operator_Del", Quoted(rOperID));
}
bool SQLP_Operator::SetLevel(const std::string& rOperID, int iLevel) {
    if (!m_pkDb) return false;
    std::string args = Quoted(rOperID); args += ","; args += F("%d", iLevel);
    return m_pkDb->ExecProc("p_Operator_SetLevel", args);
}
bool SQLP_Operator::SetPW(const std::string& rOperID, const std::string& rOldPW, const std::string& rNewPW) {
    if (!m_pkDb) return false;
    std::string args = Quoted(rOperID);
    args += ","; args += Quoted(rOldPW);
    args += ","; args += Quoted(rNewPW);
    return m_pkDb->ExecProc("p_Operator_SetPW", args);
}

// ===========================================================================
//  SQLP_Options     (DB: Options)
// ===========================================================================

bool SQLP_Options::GetInt(const std::string& rKey, int32& iOut) {
    iOut = 0;
    if (!m_pkDb) return false;
    std::string kSql = "SELECT "; kSql += DB::tGameOptionInit::kDefaultValue;
    kSql += " FROM "; kSql += DB::tGameOptionInit::kTable;
    kSql += " WHERE "; kSql += DB::tGameOptionInit::kDescription; kSql += "="; kSql += Quoted(rKey);
    kSql += " AND ";   kSql += DB::tGameOptionInit::kIsActive;    kSql += "=1";
    std::vector<DBRecord> rows;
    if (!m_pkDb->Query(kSql, rows) || rows.empty()) return false;
    iOut = rows[0].GetInt(0); return true;
}
bool SQLP_Options::GetString(const std::string& rKey, std::string& rOut) {
    rOut.clear();
    if (!m_pkDb) return false;
    std::string kSql = "SELECT "; kSql += DB::tGameOptionInit::kDescription;
    kSql += " FROM "; kSql += DB::tGameOptionInit::kTable;
    kSql += " WHERE "; kSql += DB::tGameOptionInit::kOptionNo; kSql += "="; kSql += Quoted(rKey);
    std::vector<DBRecord> rows;
    if (!m_pkDb->Query(kSql, rows) || rows.empty()) return false;
    rOut = rows[0].Get(0); return true;
}

// ===========================================================================
//  SQLP_Report / SQLP_IPChecker
// ===========================================================================

void SQLP_Report::File(const std::string& rKind, const std::string& rPayload) {
    if (!m_pkDb) return;
    std::string args = Quoted(rKind); args += ","; args += Quoted(rPayload);
    m_pkDb->ExecProc("up_Server_Report_File", args);
}

bool SQLP_IPChecker::IsBlocked(const std::string& rIP) {
    if (!m_pkDb) return false;
    std::vector<DBRecord> rows;
    if (!m_pkDb->QueryProc("up_Server_IPChecker_IsBlocked", Quoted(rIP), rows)) return false;
    return Col0I32(rows) != 0;
}

// ===========================================================================
//  SQLP_Estate -- mini-house ownership and furniture persistence.
// ===========================================================================
bool SQLP_Estate::Create(CharID owner, uint32 uiHouseId, uint32 uiTier) {
    if (!m_pkDb) return false;
    return m_pkDb->ExecProc("p_Estate_Create",
        F("%u,%u,%u", owner, uiHouseId, uiTier));
}

bool SQLP_Estate::Demolish(CharID owner) {
    if (!m_pkDb) return false;
    return m_pkDb->ExecProc("p_Estate_Demolish", F("%u", owner));
}

bool SQLP_Estate::Save(CharID owner, const uint8* pData, size_t uiLen) {
    if (!m_pkDb || !pData) return false;
    // Encode the placement blob as a hex literal (0x...). Each placement
    // is 24 bytes -- a 30-furniture house fits in 720B + 4B header, well
    // within most ODBC drivers' 8000-char varbinary limit.
    std::string hex; hex.reserve(2 + uiLen * 2);
    hex += "0x";
    static const char* kHex = "0123456789ABCDEF";
    for (size_t i = 0; i < uiLen; ++i) {
        hex += kHex[(pData[i] >> 4) & 0xF];
        hex += kHex[ pData[i]       & 0xF];
    }
    if (uiLen == 0) hex = "0x00";
    std::string args = F("%u,", owner); args += hex;
    return m_pkDb->ExecProc("p_Estate_SaveFurniture", args);
}

bool SQLP_Estate::Load(CharID owner, std::vector<DBRecord>& rOut) {
    if (!m_pkDb) return false;
    return m_pkDb->QueryProc("p_Estate_LoadFurniture", F("%u", owner), rOut);
}

} // namespace fiesta
