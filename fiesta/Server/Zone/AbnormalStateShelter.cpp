// Server/Zone/AbnormalStateShelter.cpp
// Writes persistent AbStates to CharDB on logout; loads them on login.
// WIRE-16 (Lyra, May 2026): lifted from anonymous namespace.
// Pass 6 (Feb 2026): wired to CharDBClient AbState* persistence path.
#include "AbnormalStateShelter.h"
#include "CharDBClient.h"
namespace fiesta {
void AbnormalStateShelter::Save(uint32 cid, uint32 uiAbStateId, uint32 uiRemainMs) {
    // Persist a single (cid, abState, remainMs) tuple. The CharDB exe
    // upserts against tCharAbState keyed by (cid, abState); a remainMs
    // of 0 deletes the row. Caller is responsible for filtering out
    // states tagged as non-persistent (combat-only) before this call.
    CharDBClient::Get().AbStateSet(cid, uiAbStateId, uiRemainMs);
}
void AbnormalStateShelter::Load(uint32 cid) {
    // Async query; the response arrives through CharDBClient's response
    // dispatcher and is hydrated back into the player's `AbnormalState`
    // book by the login finishing path.
    CharDBClient::Get().AbStateGetAll(cid);
}
} // namespace fiesta
