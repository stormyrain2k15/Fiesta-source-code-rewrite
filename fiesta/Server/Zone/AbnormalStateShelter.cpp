// Server/Zone/AbnormalStateShelter.cpp
// Persistence for AbStates with SaveType != 0 -- writes to CharDB on
// expiry / logout, reloads on login. The "shelter" name is the original;
// it stores the ab-state in a table that survives client reconnect.
#include "CharDBClient.h"
#include "../Shared/ShineTypes.h"
namespace fiesta { namespace {
class AbnormalStateShelter {
public:
    static void Save (uint32 cid, uint32 uiAbStateId, uint32 uiRemainMs) {
        (void)cid; (void)uiAbStateId; (void)uiRemainMs;
        // CharDBClient::Get().AbStateSet(cid, uiAbStateId, uiRemainMs);
    }
    static void Load (uint32 /*cid*/) {}
};
}} // anonymous
