// Server/Zone/KQContribute.cpp
// Per-player KQ contribution -- damage dealt + objectives completed,
// used as the weight for reward distribution at KQ end.
#include "../Shared/ShineTypes.h"
#include <map>
namespace fiesta { namespace {
class KQContribute {
public:
    static KQContribute& Get() { static KQContribute s; return s; }
    void Add(uint32 uiKQID, uint32 cid, uint32 uiPoints) { m_kPer[(uint64)uiKQID<<32|cid] += uiPoints; }
    uint32 Get_(uint32 uiKQID, uint32 cid) const {
        std::map<uint64, uint32>::const_iterator it = m_kPer.find((uint64)uiKQID<<32|cid);
        return (it == m_kPer.end()) ? 0 : it->second;
    }
private:
    std::map<uint64, uint32> m_kPer;
};
}} // anonymous
