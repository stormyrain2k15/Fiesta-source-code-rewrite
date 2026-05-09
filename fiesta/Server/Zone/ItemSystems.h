// Server/Zone/ItemSystems.h
// 16 + 17 + 18 -- upgrade / use / drop / mall / charged effects.
// EVIDENCE: PDB_CONFIRMED  symbol: ShineItemUpgrade, AccUpGradeDataBox, ItemRandomOption,
//                                  SetItemData, WeaponTitle, ShineItemUse, ChargedItem,
//                                  ChargedItemEffectDataBox, ChargedItemEffectList, ItemMall,
//                                  ItemDropFromMob, ItemDropTable, DropItemAnalyzer,
//                                  DropLocationManager, ItemLotInspector, BelongDiceTable,
//                                  RewardInven, ItemsInChest
#ifndef FIESTA_ZONE_ITEMSYSTEMS_H
#define FIESTA_ZONE_ITEMSYSTEMS_H
#include "Inventory.h"

namespace fiesta {

// 16 Upgrade / Random options / Set
class ShineItemUpgrade {
public:
    // Returns true on success; on failure, lvl is unchanged. Local provisional rates inline.
    static bool TryUpgrade(ShineItem& rItem);
};
class AccUpGradeDataBox { public: static int32 SuccessRatePct(uint16 uiCurLvl); };
class BRAccUpgradeDataBox { public: static int32 BreakRatePct(uint16 uiCurLvl); };
class ItemRandomOption  { public: static void Roll(ShineItem& rItem); };
class SetItemData       { public: static int32 SetEffectStat(uint32 uiSetId, uint16 uiPiecesEquipped); };
class WeaponTitle       { public: static uint32 ResolveTitle(uint32 uiKills); };

// 17 Use / charged / mall
class ShineItemUse {
public:
    static bool TryUse(ShinePlayer* pk, ShineItem& rItem);
};
class ChargedItem        { public: static void Tick(ShinePlayer* pk); };
class ChargedItemEffectDataBox { public: static const ChargedItemEffectRec* Find(uint32 uiInxName); };
class ItemMall          { public: static bool BuyById(ShinePlayer* pk, uint32 uiMallId, uint16 uiQty); };

// 18 Drop / loot / belong
struct DropEntry { uint32 uiInxName; uint32 uiWeight; uint16 uiQty; };
class ItemDropTable { public: static void Get(MobID s, std::vector<DropEntry>& rOut); };
class ItemDropFromMob {
public:
    static void Trigger(ShineMob* pkMob, ShinePlayer* pkKiller);
};
class DropItemAnalyzer  { public: static void Analyze(MobID s, std::vector<DropEntry>& rOut); };
class DropLocationManager { public: static Vec3 ScatterAround(const Vec3& kCenter, uint32 uiSeed); };
class BelongDiceTable   { public: static CharID PickWinner(const std::vector<CharID>& rGroup); };
class RewardInven       { public: static void Push(ShinePlayer* pk, ItemID uiItem, uint16 uiQty); };

} // namespace fiesta
#endif
