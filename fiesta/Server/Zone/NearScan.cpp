// Server/Zone/NearScan.cpp
// Spatial-proximity scan helpers. Lifted out of the anonymous-namespace
// stub so AI scripts and Lua bindings can call NearScan::Players /
// NearScan::RunOnField by name.
#include "NearScan.h"
#include "ShineObject.h"
#include "ZoneServer.h"
#include "MapField.h"

namespace fiesta {

void NearScan::Players(uint16 uiMap, float cx, float cy, float r,
                       std::vector<ShinePlayer*>& rOut) {
    rOut.clear();
    const std::map<Handle, ShinePlayer*>& kAll = ZoneServer::Get().Players();
    const float r2 = r * r;
    for (std::map<Handle, ShinePlayer*>::const_iterator it = kAll.begin();
         it != kAll.end(); ++it) {
        ShinePlayer* p = it->second; if (!p) continue;
        if (p->GetMap() != uiMap) continue;
        const float dx = p->GetX() - cx, dy = p->GetZ() - cy;
        if (dx*dx + dy*dy <= r2) rOut.push_back(p);
    }
}

void NearScan::RunOnField(Field& rField, const Vec3& kFrom,
                          std::vector<ShineObject*>& rOut)
{
    rOut.clear();
    const float kVisR = 30.0f;
    const float r2    = kVisR * kVisR;
    const std::vector<ShineObject*>& kAll = rField.Objects();
    for (size_t i = 0; i < kAll.size(); ++i) {
        ShineObject* o = kAll[i]; if (!o) continue;
        const float dx = o->GetX() - kFrom.x;
        const float dy = o->GetZ() - kFrom.z;
        if (dx*dx + dy*dy <= r2) rOut.push_back(o);
    }
}

} // namespace fiesta
