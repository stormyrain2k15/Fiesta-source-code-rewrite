// Server/Zone/Network/ZoneIPContainer.cpp
// In-memory map of (zoneId -> ip:port) pulled from ServerInfo.txt.
#include "../../Shared/ShineTypes.h"
#include <map>
#include <string>
namespace fiesta {
class ZoneIPContainer {
public:
    static ZoneIPContainer& Get() { static ZoneIPContainer s; return s; }
    void Set(uint16 zid, const std::string& ip, uint16 port) {
        m_kIp[zid] = ip; m_kPort[zid] = port;
    }
    const std::string& Ip  (uint16 zid) const { return m_kIp[zid]; }
    uint16             Port(uint16 zid) const {
        std::map<uint16, uint16>::const_iterator it = m_kPort.find(zid);
        return (it == m_kPort.end()) ? 0 : it->second;
    }
private:
    mutable std::map<uint16, std::string> m_kIp;
    std::map<uint16, uint16>              m_kPort;
};
} // namespace fiesta
