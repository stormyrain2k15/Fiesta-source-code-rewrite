// Server/Zone/Engines/ItemEngine.h
// Auto-generated: ItemEngine — aggregates 17 SHN loader(s).
#ifndef SHINE_ZONE_ENGINES_ITEMENGINE_H
#define SHINE_ZONE_ENGINES_ITEMENGINE_H
#include "../../DataReader/SHN/ActionEffectItem.h"
#include "../../DataReader/SHN/CollectCardDropRate.h"
#include "../../DataReader/SHN/ItemAction.h"
#include "../../DataReader/SHN/ItemDismantle.h"
#include "../../DataReader/SHN/ItemDropLog.h"
#include "../../DataReader/SHN/ItemInvenDel.h"
#include "../../DataReader/SHN/ItemMerchantInfo.h"
#include "../../DataReader/SHN/ItemMix.h"
#include "../../DataReader/SHN/ItemMoney.h"
#include "../../DataReader/SHN/ItemOptions.h"
#include "../../DataReader/SHN/ItemPackage.h"
#include "../../DataReader/SHN/ItemServerEquipTypeInfo.h"
#include "../../DataReader/SHN/ItemShop.h"
#include "../../DataReader/SHN/ItemSort.h"
#include "../../DataReader/SHN/ItemUpgrade.h"
#include "../../DataReader/SHN/ItemUseEffect.h"
#include "../../DataReader/SHN/MarketSearchInfo.h"
#include <vector>
#include <string>

namespace shine {

class ItemEngine {
public:
    static ItemEngine& Get();
    void Bind();

    // Per-SHN accessors
    ActionEffectItemShn& actionEffectItem() { return ActionEffectItemShn::Get(); }
    CollectCardDropRateShn& collectCardDropRate() { return CollectCardDropRateShn::Get(); }
    ItemActionShn& itemAction() { return ItemActionShn::Get(); }
    ItemDismantleShn& itemDismantle() { return ItemDismantleShn::Get(); }
    ItemDropLogShn& itemDropLog() { return ItemDropLogShn::Get(); }
    ItemInvenDelShn& itemInvenDel() { return ItemInvenDelShn::Get(); }
    ItemMerchantInfoShn& itemMerchantInfo() { return ItemMerchantInfoShn::Get(); }
    ItemMixShn& itemMix() { return ItemMixShn::Get(); }
    ItemMoneyShn& itemMoney() { return ItemMoneyShn::Get(); }
    ItemOptionsShn& itemOptions() { return ItemOptionsShn::Get(); }
    ItemPackageShn& itemPackage() { return ItemPackageShn::Get(); }
    ItemServerEquipTypeInfoShn& itemServerEquipTypeInfo() { return ItemServerEquipTypeInfoShn::Get(); }
    ItemShopShn& itemShop() { return ItemShopShn::Get(); }
    ItemSortShn& itemSort() { return ItemSortShn::Get(); }
    ItemUpgradeShn& itemUpgrade() { return ItemUpgradeShn::Get(); }
    ItemUseEffectShn& itemUseEffect() { return ItemUseEffectShn::Get(); }
    MarketSearchInfoShn& marketSearchInfo() { return MarketSearchInfoShn::Get(); }

private:
    ItemEngine() {}
};

} // namespace shine
#endif // SHINE_ZONE_ENGINES_ITEMENGINE_H
