// Server/Zone/AbnormalStateShelter.cpp
// Real save / load of active AbState rows. The save trigger picks rows
// based on the AbState row's AbStateSaveType column joined against the
// AbStateSaveTypeInfo table (loaded from `AbStateSaveTypeInfo.shn` at
// boot). `Apply` (link / die / logoff) walks the ledger once, computes
// per-row remain-ms from `uiExpireMs - now`, and pushes one row at a
// time through `CharDBClient::AbStateSet`.
//
// Important design call: a save with remain-ms == 0 is a delete (the
// CharDB stored proc is upsert-with-zero-deletes). That keeps the
// hydration path symmetric with state-removal events that fire while
// offline.
#include "AbnormalStateShelter.h"
#include "AbState.h"
#include "AbStateRuntime.h"
#include "AbnormalStateDictionary.h"
#include "GroupTables.h"
#include "MoreTables.h"           // AbStateSaveTypeInfoTable
#include "ShineObject.h"
#include "CharDBClient.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

void AbnormalStateShelter::Save(uint32 cid, uint32 uiAbStateId,
                                uint32 uiRemainMs) {
    CharDBClient::Get().AbStateSet(cid, uiAbStateId, uiRemainMs);
}

void AbnormalStateShelter::Load(uint32 cid) {
    CharDBClient::Get().AbStateGetAll(cid);
}

static bool TriggerAllowed(uint32 uiSaveType, eAbStateShelterTrigger e) {
    const AbStateSaveTypeInfoTable::Row* p =
        AbStateSaveTypeInfoTable::Get().Find(uiSaveType);
    if (!p) return false;
    if (e == SHELTER_LINK)   return p->bSaveLink;
    if (e == SHELTER_DIE)    return p->bSaveDie;
    if (e == SHELTER_LOGOFF) return p->bSaveLogoff;
    return false;
}

void AbnormalStateShelter::OnTrigger(ShinePlayer* pkPlayer,
                                     eAbStateShelterTrigger eTrig) {
    if (!pkPlayer) return;
    uint32 cid = pkPlayer->GetCharID();
    uint64 now = GTimer::NowMillis();
    const std::vector<AbStateRuntimeRow>& rows = pkPlayer->AbState().RuntimeRows();
    uint32 saved = 0;
    for (size_t i = 0; i < rows.size(); ++i) {
        const AbStateRow* pkAb =
            AbnormalStateDictionary::Get().GetRow(rows[i].uiAbStateId);
        if (!pkAb) continue;
        if (!TriggerAllowed(pkAb->uiAbStateSaveType, eTrig)) continue;
        uint32 remain = 0;
        if (rows[i].uiExpireMs > now)
            remain = (uint32)(rows[i].uiExpireMs - now);
        // Skip rows that are about to expire anyway (< 1s); they're
        // not worth a round-trip.
        if (remain < 1000) continue;
        Save(cid, rows[i].uiAbStateId, remain);
        ++saved;
    }
    SHINELOG_INFO("AbStateShelter: saved %u rows for cid=%u trigger=%u",
                  saved, cid, (uint32)eTrig);
}

bool AbnormalStateShelter::ApplyLoaded(ShinePlayer* pkPlayer,
                                       uint32 uiAbStateId,
                                       uint32 uiStrength,
                                       uint32 uiRemainMs) {
    if (!pkPlayer || uiRemainMs == 0) return false;
    return pkPlayer->AbState().ApplyAt(uiAbStateId, uiStrength,
                                       (int32)uiRemainMs);
}

} // namespace shine
