// Server/Common/ProtocolAnalysis.cpp
#include "ProtocolAnalysis.h"
#include <stdio.h>

namespace shine {

ProtocolAnalysis::ProtocolAnalysis() { InitializeCriticalSection(&m_kCs); }
ProtocolAnalysis::~ProtocolAnalysis() { DeleteCriticalSection(&m_kCs); }

ProtocolAnalysis& ProtocolAnalysis::Get() { static ProtocolAnalysis s; return s; }

void ProtocolAnalysis::Tally(NCOpcode nc, size_t in, size_t out) {
    EnterCriticalSection(&m_kCs);
    Stat& s = m_kStats[nc];
    s.uiCount += 1;
    s.uiBytesIn += in;
    s.uiBytesOut += out;
    LeaveCriticalSection(&m_kCs);
}

void ProtocolAnalysis::Dump(const char* szPath) {
    FILE* fp = NULL; fopen_s(&fp, szPath, "wb");
    if (!fp) return;
    EnterCriticalSection(&m_kCs);
    fprintf(fp, "opcode,count,bytes_in,bytes_out\n");
    for (std::map<NCOpcode, Stat>::const_iterator it = m_kStats.begin(); it != m_kStats.end(); ++it) {
        fprintf(fp, "0x%04X,%llu,%llu,%llu\n", it->first,
                (unsigned long long)it->second.uiCount,
                (unsigned long long)it->second.uiBytesIn,
                (unsigned long long)it->second.uiBytesOut);
    }
    LeaveCriticalSection(&m_kCs);
    fclose(fp);
}

} // namespace shine
