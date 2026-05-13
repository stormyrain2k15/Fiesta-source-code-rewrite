// Server/Zone/Engines/PupEngine.cpp
// Auto-generated: PupEngine — calls Load() on each constituent SHN.
#include "PupEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

PupEngine& PupEngine::Get() { static PupEngine s; return s; }

void PupEngine::Bind() {
    MinimonAutoUseItemShn::Get().Load();
    MinimonInfoShn::Get().Load();
    PupCaseShn::Get().Load();
    PupCaseDescShn::Get().Load();
    PupFactorConditionShn::Get().Load();
    PupMainShn::Get().Load();
    PupMindShn::Get().Load();
    PupPriorityShn::Get().Load();
    PupServerShn::Get().Load();
    SHINELOG_INFO("PupEngine::Bind done");
}

} // namespace shine
