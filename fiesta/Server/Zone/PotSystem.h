// Server/Zone/PotSystem.h
// 15 -- Inventory-consumed potion items.
// EVIDENCE: PDB_CONFIRMED  symbol: ItemUseFunctionTable, PotionUse, RestExp
//
// Distinct from SoulStoneSystem (which is a 0-inventory-space, vendor-bought,
// per-tier-by-level concept). "Pot" here = ItemUseFunction rows that produce
// HP, SP, EXP, or AbState changes when an inventory item is consumed.
//
// All values are sourced from `WorldTables::ItemUseFunctionTable` -- the
// constants below are clamps / common-sense bounds, not authoritative
// numbers.
#ifndef FIESTA_ZONE_POTSYSTEM_H
#define FIESTA_ZONE_POTSYSTEM_H
#include "ShineObject.h"
#include <vector>

namespace fiesta {

enum ePotKind {
    POT_NONE     = 0,
    POT_HP_FLAT  = 1,
    POT_SP_FLAT  = 2,
    POT_HP_PCT   = 3,
    POT_SP_PCT   = 4,
    POT_EXP_PCT  = 5,
    POT_REST_EXP = 6,    // rested-EXP buff (applied to next mob kills)
    POT_ABSTATE  = 7     // grants an abstate (e.g. scroll buffs)
};

struct PotDef {
    ePotKind eKind;
    int32    iAmount;        // for FLAT: HP/SP delta. PCT: 0..100. EXP_PCT: 0..100.
    uint32   uiAbStateInxName;  // for POT_ABSTATE
    uint32   uiCooldownMs;
};

// Single home for the per-character cooldown clocks. Each `ePotKind` has its
// own clock so an HP pot doesn't gate an SP pot.
class PotSystem {
public:
    static PotSystem& Get();

    // Try to consume one of `uiItemId` from `pk`'s inventory.
    // Returns true on success; false if not in inventory, on cooldown,
    // already at full hp/sp, etc.
    bool Use(ShinePlayer* pk, uint32 uiItemId);

    // Used by ItemUseFunctionTable loader to register what each item does.
    // Multiple registrations for the same item simply overwrite -- the
    // last-loaded SHN wins, matching the data-table behavior.
    void Register(uint32 uiItemId, const PotDef& rDef);

    bool IsKnown(uint32 uiItemId) const;
private:
    PotSystem();
    struct Slot { uint32 uiItemId; PotDef kDef; };
    std::vector<Slot> m_kKnown;
    // Per-(charId, kind) cooldown clock.
    struct Cooldown { CharID c; ePotKind eKind; uint64 uiReadyAtMs; };
    std::vector<Cooldown> m_kCooldowns;
    bool      OnCooldown   (CharID c, ePotKind eKind, uint64 uiNowMs) const;
    void      ArmCooldown  (CharID c, ePotKind eKind, uint64 uiUntil);
    const PotDef* FindDef  (uint32 uiItemId) const;
};

} // namespace fiesta
#endif
