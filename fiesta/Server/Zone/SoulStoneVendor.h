// Server/Zone/SoulStoneVendor.h
// vendor flow for NPCs with `Role=Merchant RoleArg0=SoulStone`.
// Two SKUs only:
//   * "HP Soul Stone"  (price = SoulStoneSystem::PriceForLevel(buyer.level))
//   * "SP Soul Stone"  (price = same)
// Both purchases increment the matching counter on SoulStoneCounts (capped
// at kSoulStoneMaxCount) and debit the player's Vis (Money). They are NOT
// added as inventory items.
// Vendor list (verbatim from World/NPC.txt SoulStone-flagged rows):
//   RouSoulMctJulia, EldSoulMctAvon, UruSoulPooring, HednisSoulKeroll,
//   StoneMctTomson, SoulMctChloe, AlruinSoulRunadilla, BeraSoulOlivia,
//   TempSoul, ... (use ShineNPCTable::FindNPC + RoleArg0 check).
//                           DATA_CONFIRMED  World/NPC.txt SoulStone vendors.
#ifndef FIESTA_ZONE_SOULSTONEVENDOR_H
#define FIESTA_ZONE_SOULSTONEVENDOR_H
#include "SoulStoneSystem.h"
#include "Inventory.h"

namespace fiesta {

class ShinePlayer;

enum eSoulStoneBuyResult {
    SS_BUY_OK             = 0,
    SS_BUY_NOT_VENDOR     = 1,    // NPC isn't a soul-stone merchant
    SS_BUY_OUT_OF_RANGE   = 2,    // too far from the NPC
    SS_BUY_NO_MONEY       = 3,
    SS_BUY_AT_CAP         = 4,    // already holds kSoulStoneMaxCount
    SS_BUY_BAD_QTY        = 5
};

class SoulStoneVendor {
public:
    // Returns true if this NPC's Role/RoleArg0 declares it a soul-stone vendor.
    static bool IsVendor(const std::string& rNpcMobName);

    // Buy `uiQty` HP or SP soul stones from the named NPC. Pays
    // PriceForLevel(player.level) per stone out of the player's inventory
    // money (Vis). Adds the qty to rCounts.
    static eSoulStoneBuyResult Buy(ShinePlayer*       pkP,
                                    const std::string& rNpcMobName,
                                    Inventory&         rInv,
                                    SoulStoneCounts&   rCounts,
                                    bool               bIsHp,
                                    uint16             uiQty);
};

} // namespace fiesta
#endif
