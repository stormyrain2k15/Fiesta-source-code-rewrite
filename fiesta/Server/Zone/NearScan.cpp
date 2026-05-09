// Server/Zone/NearScan.cpp
// Spatial-proximity scan helper. Static utility lives anonymously here
// to avoid ODR with the existing MoveManager.h NearScan declaration.
#include "ShineObject.h"
#include "ZoneServer.h"
#include <vector>
namespace fiesta { namespace {
void NearScan_Players(uint16 uiMap, float cx, float cy, float r,
                      std::vector<ShinePlayer*>& rOut) {
    const std::map<Handle, ShinePlayer*>& kAll = ZoneServer::Get().Players();
    for (std::map<Handle, ShinePlayer*>::const_iterator it = kAll.begin();
         it != kAll.end(); ++it) {
        ShinePlayer* p = it->second; if (!p) continue;
        if (p->GetMap() != uiMap) continue;
        float dx = p->GetX() - cx, dy = p->GetZ() - cy;
        if (dx*dx + dy*dy <= r*r) rOut.push_back(p);
    }
}
}} // namespace fiesta::(anon)
