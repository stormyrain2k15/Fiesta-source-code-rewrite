// Server/Zone/Quest/QuestEvent.cpp
// Quest event triggers: per-objective resolution. Called by the various
// game-event hooks (mob kill, item pick, NPC talk, location enter) and
// applied against every active quest on the player.
#include "Quest.h"
#include "../ShineObject.h"
namespace fiesta {
class QuestEvent {
public:
    static void OnMobKill   (ShinePlayer* pkP, uint32 uiNpcID)  { Quest::Get().OnMobKill (pkP, uiNpcID); }
    static void OnItemPick  (ShinePlayer* pkP, uint32 uiItemId) { Quest::Get().OnItemPick(pkP, uiItemId); }
    static void OnNpcTalk   (ShinePlayer* /*pkP*/, uint32 /*uiNpcID*/) {}
    static void OnLocationEnter(ShinePlayer* /*pkP*/, uint16 /*uiMap*/, float /*x*/, float /*y*/) {}
};
} // namespace fiesta
