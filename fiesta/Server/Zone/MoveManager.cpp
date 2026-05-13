// Server/Zone/MoveManager.cpp
#include "MoveManager.h"
#include <math.h>

namespace shine {

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

// NearScan::Run was historically defined here; it's now provided by
// NearScan::RunOnField in NearScan.cpp. The redirect below preserves
// any TU that still resolves the old name, for free, until the rename
// reaches every call site.
void NearScan_RunLegacy(Field& rField, const Vec3& kFrom,
                        std::vector<ShineObject*>& rOut)
{
    NearScan::RunOnField(rField, kFrom, rOut);
}

} // namespace shine
