// Server/DataServer/Common/Database.h
// thin wrapper around ODBC connection. Per spec rule, we do not introduce
// a new ORM/config layer. Connection string comes from ServerInfo.txt.
#ifndef SHINE_DATABASE_H
#define SHINE_DATABASE_H
#include "../../Shared/ShineTypes.h"
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <string>

#pragma comment(lib, "odbc32.lib")

namespace shine {

class DBRecord {
public:
    std::vector<std::string> kCols;
    const std::string& Get(size_t i) const { static std::string e; return i<kCols.size()?kCols[i]:e; }
    int    GetInt   (size_t i) const { return atoi(Get(i).c_str()); }
    uint32 GetU32   (size_t i) const { return (uint32)strtoul(Get(i).c_str(), NULL, 10); }
};

class Database {
public:
    Database();
    ~Database();
    bool Connect(const std::string& rConnStr);
    void Disconnect();
    bool Exec(const std::string& rSql);                                   // INSERT/UPDATE/DELETE/EXEC
    bool Query(const std::string& rSql, std::vector<DBRecord>& rOut);     // SELECT
    bool IsConnected() const { return m_hDbc != SQL_NULL_HDBC; }

    // Escape a literal string so it is safe to embed in a SQL/proc-call body.
    // T-SQL escapes single-quote by doubling it; we additionally strip any
    // embedded NUL or non-printable byte that should not reach the wire.
    static std::string Quote(const std::string& rIn);

    // Convenience: build "{CALL <proc>(<args...>)}" and Exec/Query it.
    bool ExecProc (const std::string& rProc, const std::string& rArgs);
    bool QueryProc(const std::string& rProc, const std::string& rArgs, std::vector<DBRecord>& rOut);
private:
    SQLHENV m_hEnv;
    SQLHDBC m_hDbc;
    CRITICAL_SECTION m_kCs;
};

} // namespace shine
#endif
