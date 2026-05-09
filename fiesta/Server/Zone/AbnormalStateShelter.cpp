// Server/Zone/AbnormalStateShelter.cpp
// Writes persistent AbStates to CharDB on logout; loads them on login.
// WIRE-16 (Lyra, May 2026): lifted from anonymous namespace.
#include "AbnormalStateShelter.h"
#include "CharDBClient.h"
namespace fiesta {
void AbnormalStateShelter::Save(uint32 cid, uint32 uiAbStateId, uint32 uiRemainMs) {
    // CharDBClient::Get().AbStateSet(cid, uiAbStateId, uiRemainMs);
    // TODO: wire when CharDBClient AbStateSet proc is confirmed from .bak
    (void)cid; (void)uiAbStateId; (void)uiRemainMs;
}
void AbnormalStateShelter::Load(uint32 cid) {
    // TODO: CharDBClient::Get().AbStateGetAll(cid, callback)
    (void)cid;
}
} // namespace fiesta
