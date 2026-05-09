// Server/Zone/Quest/QuestZone.cpp
// Zone-side quest packet wiring. Hooks into NC_QUEST_* opcodes; the
// actual logic delegates to Quest.cpp.
#include "Quest.h"
namespace fiesta {
void QuestZone_OnAcceptReq  (uint32 cid, uint32 q) { (void)cid; (void)q; }
void QuestZone_OnCompleteReq(uint32 cid, uint32 q) { (void)cid; (void)q; }
void QuestZone_OnAbandonReq (uint32 cid, uint32 q) { (void)cid; (void)q; }
} // namespace fiesta
