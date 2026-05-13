// Server/Zone/Engines/AbStateEngine.cpp
// Auto-generated: AbStateEngine — calls Load() on each constituent SHN.
#include "AbStateEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

AbStateEngine& AbStateEngine::Get() { static AbStateEngine s; return s; }

void AbStateEngine::Bind() {
    AbStateSaveTypeInfoShn::Get().Load();
    ActionEffectAbStateShn::Get().Load();
    ChargedDeletableBuffShn::Get().Load();
    GroupAbStateShn::Get().Load();
    MapBuffShn::Get().Load();
    StateItemShn::Get().Load();
    StateMobShn::Get().Load();
    SHINELOG_INFO("AbStateEngine::Bind done");
}

} // namespace shine
