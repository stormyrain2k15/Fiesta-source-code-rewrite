// Server/DataServer/Common/SQLP.cpp
// All stored-procedure invocations go through {CALL ProcName(?,?...)} ODBC syntax.
// Procedure names are the original ones from World00_Character.bak -- not invented here.
// EVIDENCE: VERIFY -- procedure parameter shape needs the .bak schema review pass.
#include "SQLP.h"
#include "../../Shared/ShineLogSystem.h"
#include <vector>

namespace fiesta {

bool SQLP_Account::VerifyLogin(const std::string& u, const std::string& p, AccountID& aOut) {
    if (!m_pkDb || !m_pkDb->IsConnected()) return false;
    std::string sql = "{CALL up_Server_Account_Login('" + u + "','" + p + "')}";
    std::vector<DBRecord> kRows;
    if (!m_pkDb->Query(sql, kRows) || kRows.empty()) return false;
    aOut = kRows[0].GetU32(0);
    return aOut != 0;
}

void SQLP_Statistics::OnCharLogin(CharID c) {
    if (!m_pkDb) return;
    char sql[256]; sprintf_s(sql, sizeof(sql), "{CALL up_Server_Statistics_CharLogin(%u)}", c);
    m_pkDb->Exec(sql);
}
void SQLP_Statistics::OnCharLogout(CharID c) {
    if (!m_pkDb) return;
    char sql[256]; sprintf_s(sql, sizeof(sql), "{CALL up_Server_Statistics_CharLogout(%u)}", c);
    m_pkDb->Exec(sql);
}

void SQLP_Report::File(const std::string& kind, const std::string& payload) {
    if (!m_pkDb) return;
    std::string sql = "{CALL up_Server_Report_File('" + kind + "','" + payload + "')}";
    m_pkDb->Exec(sql);
}

bool SQLP_IPChecker::IsBlocked(const std::string& ip) {
    if (!m_pkDb) return false;
    std::string sql = "{CALL up_Server_IPChecker_IsBlocked('" + ip + "')}";
    std::vector<DBRecord> kRows;
    if (!m_pkDb->Query(sql, kRows) || kRows.empty()) return false;
    return kRows[0].GetInt(0) != 0;
}

} // namespace fiesta
