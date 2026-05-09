// Server/Zone/ItemUpgrade.cpp
// Resolves an item-upgrade attempt against ItemInfo.shn columns, then
// persists the post-attempt state through CharDBClient and acks the
// client over NC_ITEM_UPGRADE_ACK.
#include "ItemUpgrade.h"
#include "ShineObject.h"
#include "ZoneServer.h"
#include "CharDBClient.h"
#include "BattleTunables.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/ITableBase.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"
#include <string.h>

namespace fiesta {

static well512 s_kRng;

// CharDB stored-procedure option type for the enchant level. Other types
// (random options, endure) are written from their respective subsystems.
static const uint8 kItemOption_Enchant = 1;

// ---------------------------------------------------------------------------
//  Enchanter NPC session.
//  Keyed by CharID so we don't have to thread a session pointer through
//  every NPC handler. The session is single-use: every successful pick of
//  an "Upgrade" button opens a session, every NC_ITEM_UPGRADE_REQ closes
//  it. A connection drop takes the session with it -- the on-disconnect
//  path in ClientSession calls CloseSession via DetachPlayer.
// ---------------------------------------------------------------------------
namespace {
    struct EnchantSession {
        uint32 uiNpcId;
        uint8  uiKind;       // 0=any, 1=weapon, 2=armour, 3=accessory
    };
    std::map<CharID, EnchantSession>& Sessions() {
        static std::map<CharID, EnchantSession> s;
        return s;
    }
}

void ItemUpgrade::OpenSession(ShinePlayer* pkP, uint32 uiNpcId, uint8 uiKind) {
    if (!pkP) return;
    EnchantSession s; s.uiNpcId = uiNpcId; s.uiKind = uiKind;
    Sessions()[pkP->GetCharID()] = s;
}

bool ItemUpgrade::IsSessionActive(ShinePlayer* pkP, uint8* puiKindOut) {
    if (!pkP) return false;
    std::map<CharID, EnchantSession>::iterator it = Sessions().find(pkP->GetCharID());
    if (it == Sessions().end()) return false;
    if (puiKindOut) *puiKindOut = it->second.uiKind;
    return true;
}

void ItemUpgrade::CloseSession(ShinePlayer* pkP) {
    if (!pkP) return;
    Sessions().erase(pkP->GetCharID());
}

eUpgradeResult ItemUpgrade::Try(Inventory& kInv, uint32 uiItemId, bool bUseLuckStone,
                                uint16& uiNewEnchantOut, uint64& uiItemKeyOut) {
    uiNewEnchantOut = 0;
    uiItemKeyOut    = 0;

    // Find the target item.
    const std::vector<ShineItem>& vAll = kInv.All();
    int idx = -1;
    for (size_t i = 0; i < vAll.size(); ++i)
        if (vAll[i].uiItemId == uiItemId) { idx = (int)i; break; }
    if (idx < 0) return UPGRADE_BLOCKED;

    // Resolve item info row.
    const ShineItem& kItem = vAll[idx];
    const ItemInfoRow* p = ITableBase<ItemInfoRow>::ms_pkTable
        ? ITableBase<ItemInfoRow>::ms_pkTable->Find(kItem.uiInxName) : NULL;
    if (!p) return UPGRADE_BLOCKED;

    // Cap checks.
    if (p->UpLimit == 0) return UPGRADE_BLOCKED;
    if (kItem.uiEnchant >= p->UpLimit) return UPGRADE_BLOCKED;
    if (kItem.uiEnchant >= kMaxUpgradeLevel) return UPGRADE_BLOCKED;

    uiItemKeyOut    = kItem.uiDbItemKey;
    uiNewEnchantOut = kItem.uiEnchant;

    // -----------------------------------------------------------------
    //  UpResource consumption (Lyra WIRE-10).
    //  Find the first item in the inventory whose ItemInfo `uiUpResource`
    //  bucket matches this target's `UpResource` column. The match is
    //  bucket-id only, not literal item id, so any tier-equivalent
    //  upgrade stone in the bag can be used. We snapshot the resource
    //  item id BEFORE the dice roll and remove it AFTER (success or
    //  fail), so a missing-resource path returns BLOCKED and leaves
    //  the inventory untouched.
    // -----------------------------------------------------------------
    uint8  uiResourceBucket = (uint8)p->UpResource;
    uint32 uiResourceItemId = 0;     // local id within the inventory
    if (uiResourceBucket != 0) {
        for (size_t i = 0; i < vAll.size(); ++i) {
            if (vAll[i].uiItemId == uiItemId) continue; // not the target
            const ItemInfoRow* q = ITableBase<ItemInfoRow>::ms_pkTable
                ? ITableBase<ItemInfoRow>::ms_pkTable->Find(vAll[i].uiInxName) : NULL;
            if (q && (uint8)q->UpResource == uiResourceBucket && q->UpLimit == 0) {
                // UpLimit==0 distinguishes a *consumable* of this bucket
                // (a stone) from another upgradeable item that happens
                // to share the bucket id.
                uiResourceItemId = vAll[i].uiItemId;
                break;
            }
        }
        if (uiResourceItemId == 0) {
            SHINELOG_WARN("ItemUpgrade: missing UpResource bucket=%u for item=%u",
                          (uint32)uiResourceBucket, uiItemId);
            return UPGRADE_BLOCKED;
        }
    }

    // -----------------------------------------------------------------
    //  Luck-Stone consumption (Lyra WIRE-11).
    //  Identified by ItemInfo.ItemUseSkill == "LuckStone" -- the only
    //  string anchor we have without a per-server config. If no row
    //  matches we silently treat bUseLuckStone as false rather than
    //  hand out a free probability boost.
    // -----------------------------------------------------------------
    uint32 uiLuckStoneItemId = 0;
    if (bUseLuckStone) {
        for (size_t i = 0; i < vAll.size(); ++i) {
            const ItemInfoRow* q = ITableBase<ItemInfoRow>::ms_pkTable
                ? ITableBase<ItemInfoRow>::ms_pkTable->Find(vAll[i].uiInxName) : NULL;
            if (q && strncmp(q->ItemUseSkill, "LuckStone",
                             sizeof(q->ItemUseSkill)) == 0) {
                uiLuckStoneItemId = vAll[i].uiItemId;
                break;
            }
        }
        if (uiLuckStoneItemId == 0) {
            SHINELOG_WARN("ItemUpgrade: LuckStone requested but none found, ignoring flag");
            bUseLuckStone = false;
        }
    }

    // Compute success rate (per-mille).
    int32 sucRate = (int32)p->UpSucRatio;
    if (bUseLuckStone) sucRate += (int32)p->UpLuckRatio;
    sucRate = (sucRate * kUpgradeSucScalerX1k) / 1000;
    if (sucRate < 0)    sucRate = 0;
    if (sucRate > 1000) sucRate = 1000;

    // Roll.
    int32 roll = (int32)(s_kRng.NextDouble() * 1000.0);
    bool  bSuccess = (roll < sucRate);

    // Resource is consumed regardless of outcome (one stone per attempt).
    if (uiResourceItemId != 0)  kInv.Remove(uiResourceItemId);
    if (uiLuckStoneItemId != 0) kInv.Remove(uiLuckStoneItemId);

    // Locate a non-const reference to the item to mutate. The Inventory
    // exposes only a const view, so we go through Add/Remove.
    ShineItem mutated = kItem;

    if (bSuccess) {
        mutated.uiEnchant = (uint16)(mutated.uiEnchant + 1);
        kInv.Remove(uiItemId);
        kInv.Add(mutated);
        uiNewEnchantOut = mutated.uiEnchant;
        return UPGRADE_OK;
    }

    // Failure consequences -- table-driven by current +N level.
    uint8 lv = (uint8)mutated.uiEnchant;
    if (lv >= (uint8)(sizeof(kFailPenaltyAtLevel) / sizeof(kFailPenaltyAtLevel[0])))
        lv = (uint8)(sizeof(kFailPenaltyAtLevel) / sizeof(kFailPenaltyAtLevel[0]) - 1);
    uint8 penalty = kFailPenaltyAtLevel[lv];

    if (penalty == 0) {
        return UPGRADE_FAIL;            // safe tier, item untouched
    }
    if (penalty == 1) {
        if (mutated.uiEnchant > 0) mutated.uiEnchant = (uint16)(mutated.uiEnchant - 1);
        kInv.Remove(uiItemId);
        kInv.Add(mutated);
        uiNewEnchantOut = mutated.uiEnchant;
        return UPGRADE_DOWNGRADE;
    }
    // penalty == 2 -> destroy.
    kInv.Remove(uiItemId);
    return UPGRADE_DESTROY;
}

eUpgradeResult ItemUpgrade::ResolveForPlayer(ShinePlayer* pkPlayer,
                                              uint32 uiItemId, bool bUseLuckStone) {
    if (!pkPlayer) return UPGRADE_BLOCKED;

    // Anti-cheat: require an open enchanter session. The session is
    // opened by NPCSystem::HandlePick on the "Upgrade" / "Enchant" /
    // "Refine" / "EquipmentUpgrade" button and closed by this call.
    uint8 uiKind = 0;
    if (!IsSessionActive(pkPlayer, &uiKind)) {
        SHINELOG_WARN("ItemUpgrade: cid=%u sent UPGRADE_REQ with no active enchanter session",
                      pkPlayer->GetCharID());
        ClientSession* cs = pkPlayer->GetSession();
        if (cs) {
            PacketBuffer ack;
            ack.WriteU8 ((uint8)UPGRADE_BLOCKED);
            ack.WriteU32(uiItemId);
            ack.WriteU16(0);
            SendPacket(cs, NC_ITEM_UPGRADE_ACK, ack.Data(), ack.Size());
        }
        return UPGRADE_BLOCKED;
    }
    // Session consumed regardless of outcome -- one click, one roll.
    CloseSession(pkPlayer);

    uint16 uiNewEnchant = 0;
    uint64 uiItemKey    = 0;
    eUpgradeResult eRes = Try(pkPlayer->Inv(), uiItemId, bUseLuckStone,
                              uiNewEnchant, uiItemKey);

    // Persist + ack. The DB write only fires when we actually have the
    // SQL identity for the row (i.e. the original Item_Create ack came
    // back); a brand-new item that has not yet finished creation is not
    // written to so the in-flight Item_Create ack stamping wins.
    if (uiItemKey != 0) {
        switch (eRes) {
            case UPGRADE_OK:
            case UPGRADE_DOWNGRADE:
                CharDBClient::Get().ItemSetOption(uiItemKey, kItemOption_Enchant,
                                                  (int32)uiNewEnchant);
                break;
            case UPGRADE_DESTROY:
                CharDBClient::Get().ItemDelete(uiItemKey);
                break;
            case UPGRADE_FAIL:
            case UPGRADE_BLOCKED:
            default:
                break;
        }
    }

    // ACK the client. Body = [uint8 result][uint32 itemId][uint16 newEnchant].
    ClientSession* cs = pkPlayer->GetSession();
    if (cs) {
        PacketBuffer ack;
        ack.WriteU8 ((uint8)eRes);
        ack.WriteU32(uiItemId);
        ack.WriteU16(uiNewEnchant);
        SendPacket(cs, NC_ITEM_UPGRADE_ACK, ack.Data(), ack.Size());
    }

    SHINELOG_INFO("ItemUpgrade cid=%u item=%u luck=%d -> result=%u newEnchant=%u",
                  pkPlayer->GetCharID(), uiItemId, bUseLuckStone ? 1 : 0,
                  (uint32)eRes, (uint32)uiNewEnchant);
    return eRes;
}

} // namespace fiesta
