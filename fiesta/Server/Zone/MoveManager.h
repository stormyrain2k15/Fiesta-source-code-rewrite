// Server/Zone/MoveManager.h
// movement / action / visibility (NearScan).
#ifndef FIESTA_ZONE_MOVEMANAGER_H
#define FIESTA_ZONE_MOVEMANAGER_H
#include "ShineObject.h"
#include "MapField.h"

namespace fiesta {

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

class NearScan {
public:
    // exact visibility radius is per-map; provisional 30.0 units.
    static void Run(Field& rField, const Vec3& kFrom, std::vector<ShineObject*>& rOut);
};

} // namespace fiesta
#endif
