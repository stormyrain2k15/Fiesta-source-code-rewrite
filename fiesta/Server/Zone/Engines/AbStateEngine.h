// Server/Zone/Engines/AbStateEngine.h
// Auto-generated: AbStateEngine — aggregates 7 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_ABSTATEENGINE_H
#define FIESTA_ZONE_ENGINES_ABSTATEENGINE_H
#include "../../DataReader/SHN/AbStateSaveTypeInfo.h"
#include "../../DataReader/SHN/ActionEffectAbState.h"
#include "../../DataReader/SHN/ChargedDeletableBuff.h"
#include "../../DataReader/SHN/GroupAbState.h"
#include "../../DataReader/SHN/MapBuff.h"
#include "../../DataReader/SHN/StateItem.h"
#include "../../DataReader/SHN/StateMob.h"
#include <vector>
#include <string>

namespace fiesta {

class AbStateEngine {
public:
    static AbStateEngine& Get();
    void Bind();

    // Per-SHN accessors
    AbStateSaveTypeInfoShn& abStateSaveTypeInfo() { return AbStateSaveTypeInfoShn::Get(); }
    ActionEffectAbStateShn& actionEffectAbState() { return ActionEffectAbStateShn::Get(); }
    ChargedDeletableBuffShn& chargedDeletableBuff() { return ChargedDeletableBuffShn::Get(); }
    GroupAbStateShn& groupAbState() { return GroupAbStateShn::Get(); }
    MapBuffShn& mapBuff() { return MapBuffShn::Get(); }
    StateItemShn& stateItem() { return StateItemShn::Get(); }
    StateMobShn& stateMob() { return StateMobShn::Get(); }

private:
    AbStateEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_ABSTATEENGINE_H
