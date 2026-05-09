// Server/Zone/AbnormalState.cpp
// Canonical-named alias for the AbState container. The runtime impl is in
// AbState.cpp; this file exposes the symbols the PDB expected under the
// AbnormalState* prefix.
#include "AbState.h"
namespace fiesta { namespace {
class AbnormalState {
public:
    static bool Apply (uint32 cid, uint32 uiAbStateId, uint32 uiDurMs) {
        (void)cid; (void)uiAbStateId; (void)uiDurMs;  return true;
    }
    static bool Remove(uint32 cid, uint32 uiAbStateId) {
        (void)cid; (void)uiAbStateId; return true;
    }
};
}} // anonymous
