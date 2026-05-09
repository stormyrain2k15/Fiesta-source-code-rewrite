// Server/Zone/GambleHouse/GambleSystem.cpp
// Zone-side gamble session. Wraps a single player's interaction with the
// gambling house NPC -- bet placement, hand reveal, payout. The cross-
// zone authority for prize tables and coin-exchange windows lives on the
// WM (WorldManager/GambleSystemServer.cpp).
#include "../ShineObject.h"
#include "../../Shared/ShineLogSystem.h"
namespace fiesta {
class GambleSystem {
public:
    static GambleSystem& Get() { static GambleSystem s; return s; }
    bool BeginSession(ShinePlayer* pkP, uint32 uiHouseNpc) { (void)pkP; (void)uiHouseNpc; return true; }
    bool PlaceBet   (ShinePlayer* pkP, uint32 uiAmount)    { (void)pkP; (void)uiAmount;  return true; }
    bool EndSession (ShinePlayer* pkP)                     { (void)pkP; return true; }
};
} // namespace fiesta
