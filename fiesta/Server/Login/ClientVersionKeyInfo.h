// Server/Login/ClientVersionKeyInfo.h
// 04 -- single-token version key file used by Login to validate the client
// build during NC_USER_CLIENT_VERSION_CHECK_REQ. File format is a 14-character
// date+revision token followed by newline (e.g., "10022024000000").
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied ClientVersionKeyInfo.txt
#ifndef FIESTA_LOGIN_CLIENTVERSIONKEYINFO_H
#define FIESTA_LOGIN_CLIENTVERSIONKEYINFO_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class ClientVersionKeyInfo {
public:
    static ClientVersionKeyInfo& Get();
    bool   Load(const char* szPath);                  // returns false if missing
    const std::string& Key() const { return m_kKey; }
    bool   IsAcceptable(const std::string& rClientKey) const;
private:
    ClientVersionKeyInfo() {}
    std::string m_kKey;
};

} // namespace fiesta
#endif
