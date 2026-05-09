// Server/Zone/Network/ProtocolAnalysis.cpp
// Per-opcode telemetry (count, last 1000-ms histogram). Used to spot
// unexpected client opcode floods.
#include "../../Shared/ShineTypes.h"
#include <map>
namespace fiesta {
class ProtocolAnalysis {
public:
    static ProtocolAnalysis& Get() { static ProtocolAnalysis s; return s; }
    void OnPacket(uint16 uiOp) { ++m_kCounts[uiOp]; }
    uint64 GetCount(uint16 uiOp) const {
        std::map<uint16, uint64>::const_iterator it = m_kCounts.find(uiOp);
        return (it == m_kCounts.end()) ? 0 : it->second;
    }
private:
    std::map<uint16, uint64> m_kCounts;
};
} // namespace fiesta
