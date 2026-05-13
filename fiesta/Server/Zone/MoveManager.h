// Server/Zone/MoveManager.h
// movement / action / visibility.
#ifndef SHINE_ZONE_MOVEMANAGER_H
#define SHINE_ZONE_MOVEMANAGER_H
#include "ShineObject.h"
#include "MapField.h"
#include "NearScan.h"           // canonical NearScan class lives there

namespace shine {

enum MoveKind { MOVE_STOP = 0, MOVE_WALK = 1, MOVE_RUN = 2, MOVE_JUMP = 3 };

class MoveManager {
public:
    static bool Move(ShineObject* pk, const Vec3& kTo, MoveKind eKind);
    static void Stop(ShineObject* pk);
};

class Action {
public:
    static bool ValidateTarget(ShineObject* pkActor, ShineObject* pkTarget);
};

// NearScan was historically declared here too. Its canonical home is
// now NearScan.h (also reachable via this header's include above).
// The legacy `NearScan::Run(Field&, Vec3, vector<ShineObject*>)` entry
// is implemented as `NearScan::RunOnField` in NearScan.cpp.

} // namespace shine
#endif
