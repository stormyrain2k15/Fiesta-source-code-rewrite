// Server/DataServer/Common/SQLP.h
// 03 -- SQLP_<area> handler base. Each area (Account, Statistics, Report, IPChecker)
// owns its module of stored-procedure calls. Module bodies execute the proc names
// already used by the original World00_Character.bak schema -- do not invent new
// table/proc names here.
// EVIDENCE: PDB_CONFIRMED  symbol: SQLP_Account, SQLP_Statistics, SQLP_Report, SQLP_IPChecker
#ifndef FIESTA_SQLP_H
#define FIESTA_SQLP_H
#include "Database.h"

namespace fiesta {

class SQLP_Base {
public:
    explicit SQLP_Base(Database* pkDb) : m_pkDb(pkDb) {}
    virtual ~SQLP_Base() {}
protected:
    Database* m_pkDb;
};

class SQLP_Account     : public SQLP_Base { public: SQLP_Account    (Database* d) : SQLP_Base(d) {} bool VerifyLogin(const std::string& u, const std::string& p, AccountID& aOut); };
class SQLP_Statistics  : public SQLP_Base { public: SQLP_Statistics (Database* d) : SQLP_Base(d) {} void OnCharLogin(CharID c); void OnCharLogout(CharID c); };
class SQLP_Report      : public SQLP_Base { public: SQLP_Report     (Database* d) : SQLP_Base(d) {} void File(const std::string& kind, const std::string& payload); };
class SQLP_IPChecker   : public SQLP_Base { public: SQLP_IPChecker  (Database* d) : SQLP_Base(d) {} bool IsBlocked(const std::string& ip); };

} // namespace fiesta
#endif
