// Server/Zone/ShineObjectRoutine.cpp
#include "ShineObjectRoutine.h"
#include "ShineObject.h"
#include "ZoneServer.h"
#include "MoveManager.h"
#include "MobAIRunner.h"

namespace fiesta {

void ShineObjectRoutine::TickAll() {
    const std::map<Handle, ShinePlayer*>& kPlayers = ZoneServer::Get().Players();
    for (std::map<Handle, ShinePlayer*>::const_iterator it = kPlayers.begin();
         it != kPlayers.end(); ++it) {
        TickPlayer(it->second);
    }
}

void ShineObjectRoutine::TickPlayer(ShinePlayer* pkP) {
    if (!pkP) return;
    MoveManager::Tick(pkP);
}

void ShineObjectRoutine::TickMob(ShineMob* pkM) {
    if (!pkM) return;
    MobAIRunner::Tick(pkM);
}

void ShineObjectRoutine::TickPet(ShinePet* /*pkP*/) {}

void ShineObjectRoutine::Tick(ShineObject* pkObj) {
    if (!pkObj) return;
    switch (pkObj->GetType()) {
        case OT_PLAYER: TickPlayer((ShinePlayer*)pkObj); break;
        case OT_MOB   : TickMob   ((ShineMob*   )pkObj); break;
        case OT_PET   : TickPet   ((ShinePet*   )pkObj); break;
        default: break;
    }
}

} // namespace fiesta
