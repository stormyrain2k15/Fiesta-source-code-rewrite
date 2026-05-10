// Server/Zone/Engines/ItemEngine.cpp
// Auto-generated: ItemEngine — calls Load() on each constituent SHN.
#include "ItemEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemEngine& ItemEngine::Get() { static ItemEngine s; return s; }

void ItemEngine::Bind() {
    ActionEffectItemShn::Get().Load();
    CollectCardDropRateShn::Get().Load();
    ItemActionShn::Get().Load();
    ItemDismantleShn::Get().Load();
    ItemDropLogShn::Get().Load();
    ItemInvenDelShn::Get().Load();
    ItemMerchantInfoShn::Get().Load();
    ItemMixShn::Get().Load();
    ItemMoneyShn::Get().Load();
    ItemOptionsShn::Get().Load();
    ItemPackageShn::Get().Load();
    ItemServerEquipTypeInfoShn::Get().Load();
    ItemShopShn::Get().Load();
    ItemSortShn::Get().Load();
    ItemUpgradeShn::Get().Load();
    ItemUseEffectShn::Get().Load();
    MarketSearchInfoShn::Get().Load();
    SHINELOG_INFO("ItemEngine::Bind done");
}

} // namespace fiesta
