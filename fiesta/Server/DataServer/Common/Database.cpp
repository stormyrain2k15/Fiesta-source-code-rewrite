// Server/DataServer/Common/Database.cpp
#include "Database.h"
#include "../../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace fiesta {

Database::Database() : m_hEnv(SQL_NULL_HENV), m_hDbc(SQL_NULL_HDBC) {
    InitializeCriticalSection(&m_kCs);
}

Database::~Database() {
    Disconnect();
    DeleteCriticalSection(&m_kCs);
}

bool Database::Connect(const std::string& rConn) {
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
    SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
    SQLCHAR szOut[1024]; SQLSMALLINT sLen = 0;
    SQLRETURN r = SQLDriverConnectA(m_hDbc, NULL, (SQLCHAR*)rConn.c_str(), SQL_NTS,
                                    szOut, sizeof(szOut), &sLen, SQL_DRIVER_NOPROMPT);
    if (r != SQL_SUCCESS && r != SQL_SUCCESS_WITH_INFO) {
        SHINELOG_ERROR("Database::Connect failed (%d)", (int)r);
        SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc); m_hDbc = SQL_NULL_HDBC;
        SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv); m_hEnv = SQL_NULL_HENV;
        return false;
    }
    SHINELOG_INFO("Database connected");
    return true;
}

void Database::Disconnect() {
    if (m_hDbc != SQL_NULL_HDBC) { SQLDisconnect(m_hDbc); SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc); m_hDbc = SQL_NULL_HDBC; }
    if (m_hEnv != SQL_NULL_HENV) { SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv); m_hEnv = SQL_NULL_HENV; }
}

bool Database::Exec(const std::string& rSql) {
    if (!IsConnected()) return false;
    EnterCriticalSection(&m_kCs);
    SQLHSTMT h; SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &h);
    SQLRETURN r = SQLExecDirectA(h, (SQLCHAR*)rSql.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, h);
    LeaveCriticalSection(&m_kCs);
    return (r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO || r == SQL_NO_DATA);
}

bool Database::Query(const std::string& rSql, std::vector<DBRecord>& rOut) {
    if (!IsConnected()) return false;
    rOut.clear();
    EnterCriticalSection(&m_kCs);
    SQLHSTMT h; SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &h);
    SQLRETURN r = SQLExecDirectA(h, (SQLCHAR*)rSql.c_str(), SQL_NTS);
    if (r != SQL_SUCCESS && r != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, h);
        LeaveCriticalSection(&m_kCs);
        return false;
    }
    SQLSMALLINT nCols = 0; SQLNumResultCols(h, &nCols);
    while (SQLFetch(h) == SQL_SUCCESS) {
        DBRecord rec;
        for (SQLSMALLINT c = 1; c <= nCols; ++c) {
            char buf[1024]; SQLLEN ind = 0;
            SQLGetData(h, c, SQL_C_CHAR, buf, sizeof(buf), &ind);
            rec.kCols.push_back((ind == SQL_NULL_DATA) ? "" : buf);
        }
        rOut.push_back(rec);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, h);
    LeaveCriticalSection(&m_kCs);
    return true;
}

std::string Database::Quote(const std::string& rIn) {
    std::string out;
    out.reserve(rIn.size() + 4);
    for (size_t i = 0; i < rIn.size(); ++i) {
        char c = rIn[i];
        if (c == '\'') { out.push_back('\''); out.push_back('\''); }
        else if ((unsigned char)c < 0x20) { /* drop NUL/control */ }
        else { out.push_back(c); }
    }
    return out;
}

bool Database::ExecProc(const std::string& rProc, const std::string& rArgs) {
    std::string kSql; kSql.reserve(rProc.size() + rArgs.size() + 8);
    kSql += "{CALL "; kSql += rProc; kSql += "("; kSql += rArgs; kSql += ")}";
    return Exec(kSql);
}

bool Database::QueryProc(const std::string& rProc, const std::string& rArgs, std::vector<DBRecord>& rOut) {
    std::string kSql; kSql.reserve(rProc.size() + rArgs.size() + 8);
    kSql += "{CALL "; kSql += rProc; kSql += "("; kSql += rArgs; kSql += ")}";
    return Query(kSql, rOut);
}

} // namespace fiesta
