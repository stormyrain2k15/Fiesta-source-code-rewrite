// Server/Zone/AttackRhythm.cpp
// Real implementation -- formerly a stub in an anonymous namespace.
#include "AttackRhythm.h"
#include "../Shared/GTimer.h"

namespace shine {

AttackRhythm& AttackRhythm::Get() { static AttackRhythm s; return s; }

bool AttackRhythm::Allow(uint32 cid, uint32 sid, uint32 cdMs) {
    if (cdMs == 0) return true;
    uint64 now = GTimer::NowMillis();
    uint64 key = ((uint64)cid << 32) | sid;
    std::map<uint64, uint64>::iterator it = m_kLast.find(key);
    if (it != m_kLast.end() && now < it->second + cdMs) return false;
    m_kLast[key] = now;
    return true;
}

void AttackRhythm::Forget(uint32 cid) {
    // Erase every (cid, *) pair. Simple O(N) sweep -- only runs on
    // disconnect so the cost is negligible vs. session lifetime.
    for (std::map<uint64, uint64>::iterator it = m_kLast.begin();
         it != m_kLast.end(); /*advance inside*/) {
        if ((uint32)(it->first >> 32) == cid) m_kLast.erase(it++);
        else                                  ++it;
    }
}

} // namespace shine
