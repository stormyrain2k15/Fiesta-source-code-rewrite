#include "CraftAndPet.h"
#include "../Shared/well512.h"
namespace shine {

static well512 s_kRng;

bool GatherNProduce::BeginGather (ShinePlayer* pk, uint32) { return pk != NULL; }
bool GatherNProduce::BeginProduce(ShinePlayer* pk, uint32) { return pk != NULL; }
int32 ProdSucRateTable::Pct(uint32, uint16 lvl) { int32 v = 30 + (int32)lvl * 5; if (v > 95) v = 95; return v; }
bool  ShineItemDismantle::Try(ShinePlayer* pk, uint32) { return pk != NULL; }
bool  ItemMixData::Mix(ShinePlayer* pk, uint32) { return pk != NULL; }

bool  PetProtoFunc::Summon (ShinePlayer* pk, uint32) { return pk != NULL; }
void  PetProtoFunc::Dismiss(ShinePlayer*) {}
int32 PetRelatedSkill::OwnerStatBonus(uint16) { return 5; }
bool  MinimonDataBox::AutoUseTrigger(ShinePlayer*) { return false; }
void  MiniHouseInterior::Tick() {}

bool  MoverDataBox::Mount  (ShinePlayer* pk, uint32) { return pk != NULL; }
void  MoverDataBox::Dismount(ShinePlayer*) {}
int32 ShineObjectMover::SpeedBonusPct(uint16 t) { return 30 + (int32)t * 10; }
bool  MoverUpgradeData::Upgrade(uint32) { return s_kRng.NextRange(100) < 50; }

int32 GambleSystem::RollDice(uint16 n, uint16 f) {
    if (f == 0) return 0;
    int32 sum = 0;
    for (uint16 i = 0; i < n; ++i) sum += (int32)(s_kRng.NextRange(f) + 1);
    return sum;
}
int32 GambleDiceTaiSaiObject::PayoutFor(uint16, uint16) { return 1; }
int32 SlotMachine::Spin(int64 w) {
    uint32 r = s_kRng.NextRange(1000);
    if (r == 0)   return (int32)(w * 100); // jackpot
    if (r < 5)    return (int32)(w * 25);
    if (r < 50)   return (int32)(w * 5);
    if (r < 200)  return (int32)w;
    return 0;
}

} // namespace shine
