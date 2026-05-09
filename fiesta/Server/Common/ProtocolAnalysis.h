// Server/Common/ProtocolAnalysis.h
// 07 Network -- runtime opcode hit counter / capture aid.
// EVIDENCE: PDB_CONFIRMED  symbol: ProtocolAnalysis
#ifndef FIESTA_PROTOCOLANALYSIS_H
#define FIESTA_PROTOCOLANALYSIS_H
#include "../Shared/ShineTypes.h"
#include <map>

namespace fiesta {

class ProtocolAnalysis {
public:
    static ProtocolAnalysis& Get();
    void Tally(NCOpcode nc, size_t uiBytesIn, size_t uiBytesOut);
    void Dump(const char* szPath);
private:
    struct Stat { uint64 uiCount; uint64 uiBytesIn; uint64 uiBytesOut; };
    std::map<NCOpcode, Stat> m_kStats;
    CRITICAL_SECTION m_kCs;
    ProtocolAnalysis();
    ~ProtocolAnalysis();
};

} // namespace fiesta
#endif
