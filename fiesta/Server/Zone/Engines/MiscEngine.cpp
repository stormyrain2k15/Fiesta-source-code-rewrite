// Server/Zone/Engines/MiscEngine.cpp
#include "MiscEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MiscEngine& MiscEngine::Get() { static MiscEngine s; return s; }

void MiscEngine::Bind() {
    AdminLvSetShn::Get().Load();
    ActionViewInfoShn::Get().Load();
    AnnounceDataShn::Get().Load();
    AttendRewardShn::Get().Load();
    AttendScheduleShn::Get().Load();
    BadNameFilterShn::Get().Load();
    BelongDiceShn::Get().Load();
    BelongTypeInfoShn::Get().Load();
    ChatColorShn::Get().Load();
    DamageLvGapEVPShn::Get().Load();
    DamageLvGapPVEShn::Get().Load();
    DamageLvGapPVPShn::Get().Load();
    ErrorCodeTableShn::Get().Load();
    MsgWorldManagerShn::Get().Load();
    MultiHitTypeShn::Get().Load();
    MysteryVaultServerShn::Get().Load();
    ProduceShn::Get().Load();
    ShineRewardShn::Get().Load();
    SingleDataShn::Get().Load();
    SpamerPenaltyShn::Get().Load();
    SpamerPenaltyRuleShn::Get().Load();
    SpamerReportShn::Get().Load();
    TermExtendMatchShn::Get().Load();
    TownPortalShn::Get().Load();
    WeaponAttribShn::Get().Load();
    SHINELOG_INFO("MiscEngine::Bind done");
}

} // namespace fiesta
