// Server/Zone/Engines/PupEngine.h
// Auto-generated: PupEngine — aggregates 9 SHN loader(s).
#ifndef SHINE_ZONE_ENGINES_PUPENGINE_H
#define SHINE_ZONE_ENGINES_PUPENGINE_H
#include "../../DataReader/SHN/MinimonAutoUseItem.h"
#include "../../DataReader/SHN/MinimonInfo.h"
#include "../../DataReader/SHN/PupCase.h"
#include "../../DataReader/SHN/PupCaseDesc.h"
#include "../../DataReader/SHN/PupFactorCondition.h"
#include "../../DataReader/SHN/PupMain.h"
#include "../../DataReader/SHN/PupMind.h"
#include "../../DataReader/SHN/PupPriority.h"
#include "../../DataReader/SHN/PupServer.h"
#include <vector>
#include <string>

namespace shine {

class PupEngine {
public:
    static PupEngine& Get();
    void Bind();

    // Per-SHN accessors
    MinimonAutoUseItemShn& minimonAutoUseItem() { return MinimonAutoUseItemShn::Get(); }
    MinimonInfoShn& minimonInfo() { return MinimonInfoShn::Get(); }
    PupCaseShn& pupCase() { return PupCaseShn::Get(); }
    PupCaseDescShn& pupCaseDesc() { return PupCaseDescShn::Get(); }
    PupFactorConditionShn& pupFactorCondition() { return PupFactorConditionShn::Get(); }
    PupMainShn& pupMain() { return PupMainShn::Get(); }
    PupMindShn& pupMind() { return PupMindShn::Get(); }
    PupPriorityShn& pupPriority() { return PupPriorityShn::Get(); }
    PupServerShn& pupServer() { return PupServerShn::Get(); }

private:
    PupEngine() {}
};

} // namespace shine
#endif // SHINE_ZONE_ENGINES_PUPENGINE_H
