// Server/Zone/ChargedItem/ChargedItem.cpp
// Cash-shop "charged" buff items -- temporary stat boosts purchased
// from the item mall and consumable in fixed durations. Per-character
// active buffs persist via CharDBClient::ChargedBuffSet.
#include "../ShineObject.h"
#include "../CharDBClient.h"
#include "../../Shared/GTimer.h"

namespace fiesta {

struct ChargedBuff {
    uint32 uiBuffId;
    uint64 uiEndsAtMs;
    uint16 uiBoughtCount;
};

class ChargedItem {
public:
    static ChargedItem& Get() { static ChargedItem s; return s; }
    bool Activate(ShinePlayer* pkP, uint32 uiBuffId, uint32 uiDurMs);
    void Tick(ShinePlayer* pkP);
};

bool ChargedItem::Activate(ShinePlayer* pkP, uint32 uiBuffId, uint32 uiDurMs) {
    if (!pkP) return false;
    CharDBClient::Get().ChargedBuffSet(pkP->GetCharID(), uiBuffId,
                                        GTimer::NowMillis() + uiDurMs);
    return true;
}

void ChargedItem::Tick(ShinePlayer* /*pkP*/) {}

} // namespace fiesta
