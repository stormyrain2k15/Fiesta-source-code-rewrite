// Server/Zone/BriefInfoExchange.cpp
// On entering each other's near-scan radius, two ShineObjects exchange
// "brief info" -- minimal display payload (name, level, class, equipped
// hat/weapon model, title). Detail info is a follow-up request.
#include "ShineObject.h"
namespace fiesta {
class BriefInfoExchange {
public:
    static void OnAppear  (ShinePlayer* /*pkObserver*/, ShineObject* /*pkSubject*/) {}
    static void OnDisappear(ShinePlayer* /*pkObserver*/, ShineObject* /*pkSubject*/) {}
};
} // namespace fiesta
