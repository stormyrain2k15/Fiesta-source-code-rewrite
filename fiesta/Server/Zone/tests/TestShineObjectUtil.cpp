// Server/Zone/tests/TestShineObjectUtil.cpp
// Helpers for tests that need a synthetic ShineObject without loading
// the world.
#include "TestBase.h"
#include "../ShineObject.h"

namespace shine {

static ShinePlayer* MakePlayer(uint32 cid, uint8 cls, uint16 lv) {
    ShinePlayer* p = new ShinePlayer();
    p->SetCharID(cid);
    p->SetClass (cls);
    p->SetLevel (lv);
    return p;
}

(void)MakePlayer;  // silence "unused" warning when no test pulls it.

} // namespace shine
