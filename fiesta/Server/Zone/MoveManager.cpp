// Server/Zone/MoveManager.cpp
#include "MoveManager.h"
#include <math.h>

namespace fiesta {

// Local provisional movement caps.
static const float kMaxWalkUnitsPerCmd = 8.0f;
static const float kMaxRunUnitsPerCmd  = 16.0f;
static const float kVisibilityRadius   = 30.0f;

bool MoveManager::Move(ShineObject* pk, const Vec3& kTo, MoveKind eKind) {
    if (!pk) return false;
    Vec3 kFrom = pk->GetPos();
    float dx = kTo.x - kFrom.x, dy = kTo.y - kFrom.y, dz = kTo.z - kFrom.z;
    float dist = (float)sqrt(dx*dx + dy*dy + dz*dz);
    float kCap = (eKind == MOVE_RUN) ? kMaxRunUnitsPerCmd : kMaxWalkUnitsPerCmd;
    if (dist > kCap) return false; // anti-teleport guard
    pk->SetPos(kTo);
    return true;
}

void MoveManager::Stop(ShineObject*) {}

bool Action::ValidateTarget(ShineObject* pkA, ShineObject* pkT) {
    if (!pkA || !pkT) return false;
    if (pkA == pkT) return false;
    if (pkA->GetMapId() != pkT->GetMapId()) return false;
    return true;
}

void NearScan::Run(Field& rField, const Vec3& kFrom, std::vector<ShineObject*>& rOut) {
    rOut.clear();
    const std::vector<ShineObject*>& v = rField.Objects();
    for (size_t i = 0; i < v.size(); ++i) {
        Vec3 p = v[i]->GetPos();
        float dx = p.x - kFrom.x, dy = p.y - kFrom.y;
        if (dx*dx + dy*dy <= kVisibilityRadius * kVisibilityRadius) rOut.push_back(v[i]);
    }
}

} // namespace fiesta
