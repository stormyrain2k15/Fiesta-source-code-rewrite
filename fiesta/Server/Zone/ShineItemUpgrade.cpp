// Server/Zone/ShineItemUpgrade.cpp
// Canonical-named alias for ItemUpgrade. The real flow lives in
// ItemUpgrade.cpp (Try / ResolveForPlayer).
#include "ItemUpgrade.h"
namespace fiesta {
// The PDB has both ShineItemUpgrade and ItemUpgrade prefixes; expose a
// trivial pass-through so symbols resolve under either name.
class ShineItemUpgrade_Pdb {
public:
    static eUpgradeResult Resolve(ShinePlayer* pkP, uint32 uiItemId, bool bLuck) {
        return ItemUpgrade::ResolveForPlayer(pkP, uiItemId, bLuck);
    }
};
} // namespace fiesta
