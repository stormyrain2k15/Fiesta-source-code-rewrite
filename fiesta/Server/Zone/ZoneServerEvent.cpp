// Server/Zone/ZoneServerEvent.cpp
// Event dispatch loop. Frame ticks: AI, AbState expiry, drop-decay,
// PineScript step, MagicContainer tick, MobHatchery refill, AuctionSystem
// tick, Telemetry / SpyNet pass.
#include "ZoneServer.h"
#include "ShineObjectRoutine.h"
#include "PineScript/PineScript.h"
namespace fiesta {
class ZoneServerEvent {
public:
    static void TickAll() {
        ShineObjectRoutine::TickAll();
        PineScript::Get().Tick();
    }
};
} // namespace fiesta
