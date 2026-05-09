// Server/Shared/ServerInfo.h
// 00/01 -- per-service config (ServerInfo.txt / ZoneServerInfo.txt) reader.
// EVIDENCE: PDB_CONFIRMED  symbol: ServerInfo, ServerInfo.txt, ZoneServerInfo.txt
#ifndef FIESTA_SERVERINFO_H
#define FIESTA_SERVERINFO_H
#include "ShineTypes.h"
#include <map>
#include <string>

namespace fiesta {

class ServerInfo {
public:
    ServerInfo();
    bool Load(const char* szPath);
    const std::string& GetString(const std::string& rKey, const std::string& rDef = "") const;
    int    GetInt   (const std::string& rKey, int    nDef = 0) const;
    uint16 GetU16   (const std::string& rKey, uint16 uDef = 0) const;
    bool   GetBool  (const std::string& rKey, bool   bDef = false) const;
    const std::map<std::string, std::string>& All() const { return m_kKv; }
private:
    std::map<std::string, std::string> m_kKv;
    static std::string s_kEmpty;
};

} // namespace fiesta
#endif
