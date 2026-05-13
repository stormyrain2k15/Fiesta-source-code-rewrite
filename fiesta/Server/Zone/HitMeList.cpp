// Server/Zone/HitMeList.cpp
// Per-mob aggro / hit-list. Tracks every player/pet that has dealt
// damage to this mob, with their accumulated threat. Used to decide kill
// credit for exp + drop, and to select the next attack target.
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>
namespace shine {

struct HitEntry { uint32 uiCharNo; uint64 uiThreat; };

class HitMeList {
public:
    void Add   (uint32 cid, uint32 uiDamage) { m_kThreat[cid] += uiDamage; }
    void Clear () { m_kThreat.clear(); }
    uint32 TopThreat() const {
        uint32 cid = 0; uint64 best = 0;
        for (std::map<uint32, uint64>::const_iterator it = m_kThreat.begin();
             it != m_kThreat.end(); ++it)
            if (it->second > best) { best = it->second; cid = it->first; }
        return cid;
    }
    void Snapshot(std::vector<HitEntry>& rOut) const {
        for (std::map<uint32, uint64>::const_iterator it = m_kThreat.begin();
             it != m_kThreat.end(); ++it) {
            HitEntry e; e.uiCharNo = it->first; e.uiThreat = it->second;
            rOut.push_back(e);
        }
    }
private:
    std::map<uint32, uint64> m_kThreat;
};

} // namespace shine
