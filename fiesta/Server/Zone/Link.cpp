// Server/Zone/Link.cpp
// Zone gates -- portal points that move a player from one map to another.
// Sourced from MapLink.shn; runtime entries are spatial-indexed against
// the player's near-scan list.
#include "../DataReader/ShnRegistry.h"
#include <vector>
namespace fiesta {
struct LinkPoint { uint16 uiSrcMap; float sx, sy, r; uint16 uiDstMap; float dx, dy; };
class Link {
public:
    static Link& Get() { static Link s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MapLink") != NULL; }
    const LinkPoint* Lookup(uint16 uiMap, float x, float y) const;
private:
    std::vector<LinkPoint> m_kLinks;
};
const LinkPoint* Link::Lookup(uint16 uiMap, float x, float y) const {
    for (size_t i = 0; i < m_kLinks.size(); ++i) {
        const LinkPoint& l = m_kLinks[i];
        if (l.uiSrcMap != uiMap) continue;
        float dx = x - l.sx, dy = y - l.sy;
        if (dx*dx + dy*dy <= l.r*l.r) return &m_kLinks[i];
    }
    return NULL;
}
} // namespace fiesta
