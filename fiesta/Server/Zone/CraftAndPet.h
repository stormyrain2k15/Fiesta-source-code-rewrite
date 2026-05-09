// Server/Zone/CraftAndPet.h
//                                  PetData, PetProtoFunc, PetRelatedSkill, MinimonDataBox, MiniHouseInterior,
//                                  MoverDataBox, ShineObjectMover, MoverUpgradeData,
//                                  GambleSystem, GambleDiceTaiSaiObject, SlotMachine
#ifndef FIESTA_ZONE_CRAFTANDPET_H
#define FIESTA_ZONE_CRAFTANDPET_H
#include "Inventory.h"

namespace fiesta {

class GatherNProduce { public:
    static bool BeginGather (ShinePlayer* pk, uint32 uiNodeId);
    static bool BeginProduce(ShinePlayer* pk, uint32 uiRecipe);
};
class ProdSucRateTable    { public: static int32 Pct(uint32 uiRecipe, uint16 uiSkillLvl); };
class ShineItemDismantle  { public: static bool Try(ShinePlayer* pk, uint32 uiItemId); };
class ItemMixData         { public: static bool Mix(ShinePlayer* pk, uint32 uiRecipeId); };

struct PetData { CharID owner; uint32 uiPetItemId; uint16 uiSpecies; int32 iHunger; int32 iLoyalty; };
class PetProtoFunc      { public: static bool Summon (ShinePlayer* pk, uint32 uiPetItemId); static void Dismiss(ShinePlayer* pk); };
class PetRelatedSkill   { public: static int32 OwnerStatBonus(uint16 uiPetSpecies); };
class MinimonDataBox    { public: static bool AutoUseTrigger(ShinePlayer* pk); };
class MiniHouseInterior { public: static void Tick(); };

class MoverDataBox     { public: static bool Mount  (ShinePlayer* pk, uint32 uiMoverItemId); static void Dismount(ShinePlayer* pk); };
class ShineObjectMover { public: static int32 SpeedBonusPct(uint16 uiTier); };
class MoverUpgradeData { public: static bool Upgrade(uint32 uiMoverItemId); };

class GambleSystem        { public: static int32 RollDice(uint16 uiNDice, uint16 uiNFaces); };
class GambleDiceTaiSaiObject { public: static int32 PayoutFor(uint16 uiBetType, uint16 uiResult); };
class SlotMachine         { public: static int32 Spin(int64 iWager); };

} // namespace fiesta
#endif
