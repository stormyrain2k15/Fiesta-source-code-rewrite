// Server/Zone/ShineObjectRoutine.h
// Per-frame update dispatch over every live ShineObject in the zone.
// Splits the work into ranges so the IOCP worker pool can fan out.
#ifndef FIESTA_ZONE_SHINEOBJECT_ROUTINE_H
#define FIESTA_ZONE_SHINEOBJECT_ROUTINE_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class ShineObject;
class ShinePlayer;
class ShineMob;
class ShinePet;

class ShineObjectRoutine {
public:
    static void TickAll();                          // walks ZoneServer::Get().Objects()
    static void TickPlayer(ShinePlayer* pkP);
    static void TickMob   (ShineMob*    pkM);
    static void TickPet   (ShinePet*    pkP);
    // Generic dispatch -- looks at GetType() and routes to the typed Tick.
    static void Tick      (ShineObject* pkObj);
};

} // namespace fiesta
#endif
