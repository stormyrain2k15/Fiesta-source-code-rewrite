// Server/Zone/KQContribute.cpp
// Per-player KQ contribution. #include "KQContribute.h"
#include <map>
namespace shine {
KQContribute& KQContribute::Get() { static KQContribute s; return s; }
void KQContribute::Add(uint32 uiKQID, uint32 cid, uint32 uiPoints) {
    m_kPer[(uint64)uiKQID << 32 | cid] += uiPoints;
}
uint32 KQContribute::GetPoints(uint32 uiKQID, uint32 cid) const {
    std::map<uint64, uint32>::const_iterator it = m_kPer.find((uint64)uiKQID << 32 | cid);
    return (it == m_kPer.end()) ? 0 : it->second;
}
void KQContribute::Clear(uint32 uiKQID) {
    // Remove all entries for this KQ
    uint64 lo = (uint64)uiKQID << 32;
    uint64 hi = lo | 0xFFFFFFFFu;
    std::map<uint64, uint32>::iterator it = m_kPer.lower_bound(lo);
    while (it != m_kPer.end() && it->first <= hi) it = m_kPer.erase(it);
}
} // namespace shine
