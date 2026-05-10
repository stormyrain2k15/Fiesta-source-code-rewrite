// Server/Zone/Engines/MiscEngine.h
#ifndef FIESTA_ZONE_ENGINES_MISCENGINE_H
#define FIESTA_ZONE_ENGINES_MISCENGINE_H
#include "../../DataReader/SHN/AdminLvSet.h"
#include "../../DataReader/SHN/ActionViewInfo.h"
#include "../../DataReader/SHN/AnnounceData.h"
#include "../../DataReader/SHN/AttendReward.h"
#include "../../DataReader/SHN/AttendSchedule.h"
#include "../../DataReader/SHN/BadNameFilter.h"
#include "../../DataReader/SHN/BelongDice.h"
#include "../../DataReader/SHN/BelongTypeInfo.h"
#include "../../DataReader/SHN/ChatColor.h"
#include "../../DataReader/SHN/DamageLvGapEVP.h"
#include "../../DataReader/SHN/DamageLvGapPVE.h"
#include "../../DataReader/SHN/DamageLvGapPVP.h"
#include "../../DataReader/SHN/ErrorCodeTable.h"
#include "../../DataReader/SHN/MsgWorldManager.h"
#include "../../DataReader/SHN/MultiHitType.h"
#include "../../DataReader/SHN/MysteryVaultServer.h"
#include "../../DataReader/SHN/Produce.h"
#include "../../DataReader/SHN/ShineReward.h"
#include "../../DataReader/SHN/SingleData.h"
#include "../../DataReader/SHN/SpamerPenalty.h"
#include "../../DataReader/SHN/SpamerPenaltyRule.h"
#include "../../DataReader/SHN/SpamerReport.h"
#include "../../DataReader/SHN/TermExtendMatch.h"
#include "../../DataReader/SHN/TownPortal.h"
#include "../../DataReader/SHN/WeaponAttrib.h"
#include <vector>
#include <string>

namespace fiesta {

class MiscEngine {
public:
    static MiscEngine& Get();
    void Bind();

    // Per-SHN accessors
    AdminLvSetShn& adminLvSet() { return AdminLvSetShn::Get(); }
    ActionViewInfoShn& actionViewInfo() { return ActionViewInfoShn::Get(); }
    AnnounceDataShn& announceData() { return AnnounceDataShn::Get(); }
    AttendRewardShn& attendReward() { return AttendRewardShn::Get(); }
    AttendScheduleShn& attendSchedule() { return AttendScheduleShn::Get(); }
    BadNameFilterShn& badNameFilter() { return BadNameFilterShn::Get(); }
    BelongDiceShn& belongDice() { return BelongDiceShn::Get(); }
    BelongTypeInfoShn& belongTypeInfo() { return BelongTypeInfoShn::Get(); }
    ChatColorShn& chatColor() { return ChatColorShn::Get(); }
    DamageLvGapEVPShn& damageLvGapEVP() { return DamageLvGapEVPShn::Get(); }
    DamageLvGapPVEShn& damageLvGapPVE() { return DamageLvGapPVEShn::Get(); }
    DamageLvGapPVPShn& damageLvGapPVP() { return DamageLvGapPVPShn::Get(); }
    ErrorCodeTableShn& errorCodeTable() { return ErrorCodeTableShn::Get(); }
    MsgWorldManagerShn& msgWorldManager() { return MsgWorldManagerShn::Get(); }
    MultiHitTypeShn& multiHitType() { return MultiHitTypeShn::Get(); }
    MysteryVaultServerShn& mysteryVaultServer() { return MysteryVaultServerShn::Get(); }
    ProduceShn& produce() { return ProduceShn::Get(); }
    ShineRewardShn& shineReward() { return ShineRewardShn::Get(); }
    SingleDataShn& singleData() { return SingleDataShn::Get(); }
    SpamerPenaltyShn& spamerPenalty() { return SpamerPenaltyShn::Get(); }
    SpamerPenaltyRuleShn& spamerPenaltyRule() { return SpamerPenaltyRuleShn::Get(); }
    SpamerReportShn& spamerReport() { return SpamerReportShn::Get(); }
    TermExtendMatchShn& termExtendMatch() { return TermExtendMatchShn::Get(); }
    TownPortalShn& townPortal() { return TownPortalShn::Get(); }
    WeaponAttribShn& weaponAttrib() { return WeaponAttribShn::Get(); }

private:
    MiscEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_MISCENGINE_H
