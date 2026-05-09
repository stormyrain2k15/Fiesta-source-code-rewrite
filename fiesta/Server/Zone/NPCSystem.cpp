// Server/Zone/NPCSystem.cpp
#include "NPCSystem.h"
#include "Inventory.h"
#include "NPCItemListTable.h"
#include "GroupTables.h"
#include "ExtendedTables.h"
#include "WorldTables.h"
#include "QuestSystem.h"
#include "ItemUpgrade.h"
#include "../Common/NETCOMMAND.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>
#include <windows.h>

namespace fiesta {

NPCManager& NPCManager::Get() { static NPCManager s; return s; }

void NPCManager::Register(ShineNPC* p) { if (p) m_kAll[p->m_uiNpcId] = p; }
void NPCManager::RegisterKey(uint32 id, const std::string& rN) { m_kKey[id] = rN; }
void NPCManager::RegisterMenu(uint32 id, uint8 m, const std::string& rRole,
                              const std::string& rArg) {
    m_kMenu[id] = m; m_kRole[id] = rRole; m_kRoleArg[id] = rArg;
}
ShineNPC* NPCManager::Find(uint32 id) {
    std::map<uint32, ShineNPC*>::iterator it = m_kAll.find(id);
    return (it == m_kAll.end()) ? NULL : it->second;
}
const std::string& NPCManager::KeyOf(uint32 id) const {
    std::map<uint32, std::string>::const_iterator it = m_kKey.find(id);
    return (it == m_kKey.end()) ? m_kEmpty : it->second;
}
uint8 NPCManager::MenuOf(uint32 id) const {
    std::map<uint32, uint8>::const_iterator it = m_kMenu.find(id);
    return (it == m_kMenu.end()) ? 0 : it->second;
}
const std::string& NPCManager::RoleOf(uint32 id) const {
    std::map<uint32, std::string>::const_iterator it = m_kRole.find(id);
    return (it == m_kRole.end()) ? m_kEmpty : it->second;
}
const std::string& NPCManager::RoleArgOf(uint32 id) const {
    std::map<uint32, std::string>::const_iterator it = m_kRoleArg.find(id);
    return (it == m_kRoleArg.end()) ? m_kEmpty : it->second;
}

void NPCAct::OnClick(ShinePlayer* pk, ShineNPC* pkNpc) {
    if (!pk || !pkNpc) return;
    ServerMenuActor::OpenMenu(pk, pkNpc->m_uiNpcId);
}

// Walks the "TabXX" tables in `NPCItemList/<MobName>.txt`, resolves each
// 33-char ItemIndex via ItemTables, and projects (uiInxName, BuyPrice)
// for the merchant UI. NPCItemListBox is keyed by the mob-name registered
// at Field-spawn time.
void NPCItemList::GetForShop(uint32 uiNpcId, std::vector<NPCMenuItem>& rOut) {
    rOut.clear();
    const std::string& rKey = NPCManager::Get().KeyOf(uiNpcId);
    if (rKey.empty()) return;
    const NPCItemListFile* pkF = NPCItemListBox::Get().Find(rKey);
    if (!pkF) return;
    const std::vector<NPCShopTab>& tabs = pkF->Tabs();
    for (size_t t = 0; t < tabs.size(); ++t) {
        const NPCShopTab& tab = tabs[t];
        for (size_t r = 0; r < tab.kRows.size(); ++r) {
            const NPCShopRow& row = tab.kRows[r];
            for (size_t c = 0; c < row.kColumns.size(); ++c) {
                const std::string& s = row.kColumns[c];
                if (s.empty() || s == "-") continue;
                const ItemInfoRow* pkI = ItemTables::Get().FindByInx(s);
                if (!pkI) continue;
                NPCMenuItem mi;
                mi.uiInxName = pkI->uiID;
                mi.iPrice    = (int64)pkI->uiBuyPrice;
                rOut.push_back(mi);
            }
        }
    }
}

void ServerMenuActor::OpenMenu(ShinePlayer* pk, uint32 uiNpcId) {
    if (!pk) return;
    // 1) Resolve the NPC's menu page (ShineNPC.NPCMenu).
    uint8 page = NPCManager::Get().MenuOf(uiNpcId);
    SHINELOG_DEBUG("MenuOpen char=%u npc=%u key=%s page=%u",
                   (uint32)pk->GetCharID(), uiNpcId,
                   NPCManager::Get().KeyOf(uiNpcId).c_str(), (uint32)page);

    // 2) Walk NPCAction.txt for every NPCAction row whose NPCMenu matches.
    //    Each row pairs an NPCCondition (gating) with a ViewInfo (button)
    //    and a DialogID (the spoken text + sub-buttons).
    std::vector<const NPCActionTable::ActionRow*> rows;
    NPCActionTable::Get().ActionsForMenu(page, rows);

    // 3) Build the NC_NPC_MENU_OPEN_CMD payload.
    //    Layout:
    //      uint32 npcId
    //      uint32 dialogID    (0 if the NPC has no opening line)
    //      pstr   dialogTextKey   (localised on client via NpcDialogData)
    //      uint8  buttonCount
    //      { uint32 viewInfoId, pstr labelKey, uint32 iconId,
    //        pstr actionTag, pstr arg0, pstr arg1 } * buttonCount
    PacketBuffer body;
    body.WriteU32(uiNpcId);

    // Default "opening line" is taken from the first row's ViewInfo's
    // implied DialogID -- the client uses this to show the NPC's mood/text
    // before any button is pressed. If the row has no DialogID we leave it 0.
    uint32 openingDialog = 0;
    std::string openingText;
    for (size_t i = 0; i < rows.size(); ++i) {
        if (rows[i]->uiViewInfoId) {
            const NPCDialogTables::ViewRow* pkV =
                NPCDialogTables::Get().FindView(rows[i]->uiViewInfoId);
            if (pkV) {
                // The action tag for "Talk" doubles as the dialog id.
                if (rows[i]->kAction == "Talk") {
                    openingDialog = (uint32)strtoul(rows[i]->kArg0.c_str(), NULL, 10);
                    const NPCDialogTables::DialogRow* pkD =
                        NPCDialogTables::Get().FindDialog(openingDialog);
                    if (pkD) openingText = pkD->kTextKey;
                    break;
                }
            }
        }
    }
    body.WriteU32(openingDialog);
    body.WriteString(openingText);

    uint8 nButtons = 0;
    PacketBuffer btns;
    for (size_t i = 0; i < rows.size(); ++i) {
        const NPCActionTable::CondRow* pkC =
            NPCActionTable::Get().FindCond(rows[i]->uiConditionID);
        bool ok = true;
        if (pkC) {
            if (pkC->kConditionA == "Level") {
                if      (pkC->kTypeA == ">=") ok = pk->GetLevel() >= pkC->uiAX;
                else if (pkC->kTypeA == "<=") ok = pk->GetLevel() <= pkC->uiAX;
                else if (pkC->kTypeA == "==") ok = pk->GetLevel() == pkC->uiAX;
            }
        }
        if (!ok) continue;

        // Resolve the per-button label / icon / action tag from
        // NPCViewInfo.shn. If no view-info row exists we fall back to the
        // raw NPCAction columns so the client still gets *something*.
        const NPCDialogTables::ViewRow* pkV =
            NPCDialogTables::Get().FindView(rows[i]->uiViewInfoId);
        std::string lbl, act, a0, a1; uint32 icon = 0;
        if (pkV) {
            lbl  = pkV->kLabelKey;
            icon = pkV->uiIconID;
            act  = pkV->kActionTag;
            a0   = pkV->kArg0;
            a1   = pkV->kArg1;
        } else {
            act  = rows[i]->kAction;
            a0   = rows[i]->kArg0;
            a1   = rows[i]->kArg1;
        }
        btns.WriteU32(rows[i]->uiViewInfoId);
        btns.WriteString(lbl);
        btns.WriteU32(icon);
        btns.WriteString(act);
        btns.WriteString(a0);
        btns.WriteString(a1);
        ++nButtons;
    }
    body.WriteU8(nButtons);
    body.WriteBytes(btns.Data(), btns.Size());

    // 4) Dispatch.
    GPacket kPkt; kPkt.SetOpcode(NC_NPC_MENU_OPEN_CMD);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    if (pk->GetSession()) pk->GetSession()->SendPacket(kPkt);
}

// -----------------------------------------------------------------------------
//  Chained sub-dialog. Re-uses NC_NPC_MENU_OPEN_CMD but populates the dialog
//  text + button list from NpcDialogData(uiDialogId) / NPCViewInfo. The
//  client treats this exactly like a top-level click; the only difference is
//  the page is not driven by NPCAction rows but by the chained dialog tree.
// -----------------------------------------------------------------------------
void ServerMenuActor::SendDialog(ShinePlayer* pk, uint32 uiNpcId, uint32 uiDialogId) {
    if (!pk) return;
    PacketBuffer body;
    body.WriteU32(uiNpcId);
    body.WriteU32(uiDialogId);

    const NPCDialogTables::DialogRow* pkD =
        NPCDialogTables::Get().FindDialog(uiDialogId);
    body.WriteString(pkD ? pkD->kTextKey : std::string());

    std::vector<const NPCDialogTables::ViewRow*> kBtns;
    NPCDialogTables::Get().ButtonsFor(uiDialogId, kBtns);
    uint8 nButtons = (kBtns.size() > 255) ? 255 : (uint8)kBtns.size();
    body.WriteU8(nButtons);
    for (uint8 i = 0; i < nButtons; ++i) {
        const NPCDialogTables::ViewRow* pkV = kBtns[i];
        body.WriteU32(pkV->uiViewInfoID);
        body.WriteString(pkV->kLabelKey);
        body.WriteU32(pkV->uiIconID);
        body.WriteString(pkV->kActionTag);
        body.WriteString(pkV->kArg0);
        body.WriteString(pkV->kArg1);
    }

    GPacket kPkt; kPkt.SetOpcode(NC_NPC_MENU_OPEN_CMD);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    if (pk->GetSession()) pk->GetSession()->SendPacket(kPkt);
}

// -----------------------------------------------------------------------------
//  NC_NPC_SHOP_OPEN_CMD  -- "Trade" button. Walks the NPCItemList registered
//  for this NPC's mob-name and projects each (uiInxName, BuyPrice) pair.
//  Wire layout:
//    uint32 npcId
//    uint16 itemCount
//    { uint32 uiInxName, int64 iBuyPrice } * itemCount
// -----------------------------------------------------------------------------
void ServerMenuActor::OpenShop(ShinePlayer* pk, uint32 uiNpcId) {
    if (!pk) return;
    std::vector<NPCMenuItem> kList;
    NPCItemList::GetForShop(uiNpcId, kList);

    PacketBuffer body;
    body.WriteU32(uiNpcId);
    uint16 nItems = (kList.size() > 65535) ? 65535 : (uint16)kList.size();
    body.WriteU16(nItems);
    for (uint16 i = 0; i < nItems; ++i) {
        body.WriteU32(kList[i].uiInxName);
        body.WriteU64((uint64)kList[i].iPrice);
    }

    GPacket kPkt; kPkt.SetOpcode(NC_NPC_SHOP_OPEN_CMD);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    if (pk->GetSession()) pk->GetSession()->SendPacket(kPkt);
    SHINELOG_DEBUG("ShopOpen char=%u npc=%u items=%u",
                   (uint32)pk->GetCharID(), uiNpcId, (uint32)nItems);
}

void ServerMenuActor::SendPickAck(ShinePlayer* pk, uint32 uiNpcId,
                                  uint32 uiViewInfoId, uint8 uiResult,
                                  const std::string& rMsg) {
    if (!pk || !pk->GetSession()) return;
    // PROVISIONAL_BODY: NC_NPC_MENU_PICK_ACK body shape (npcId, viewInfoId,
    // result, msg) is a placeholder. The NA2016 client format has not
    // been captured into this tree; reconcile against a real ack
    // capture before relying on the field order downstream.
    PacketBuffer body;
    body.WriteU32(uiNpcId);
    body.WriteU32(uiViewInfoId);
    body.WriteU8 (uiResult);
    body.WriteString(rMsg);
    GPacket kPkt; kPkt.SetOpcode(NC_NPC_MENU_PICK_ACK);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    pk->GetSession()->SendPacket(kPkt);
}

// -----------------------------------------------------------------------------
//  NC_NPC_MENU_PICK_REQ.  ViewInfoId == 0 is the synthetic "open root menu"
//  click (the client uses this for the initial talk-to). Otherwise we look
//  up the ViewRow's ActionTag in NPCViewInfo and route to the right
//  subsystem.
// -----------------------------------------------------------------------------
void ServerMenuActor::HandlePick(ShinePlayer* pk, uint32 uiNpcId, uint32 uiViewInfoId) {
    if (!pk) return;

    // 0) Initial click -> render the NPC's root menu page.
    if (uiViewInfoId == 0) {
        // Bump the "Meeting" quest counters for this NPC.
        QuestEventDispatcher::Get().OnNpcTalked(pk->GetCharID(), uiNpcId);
        OpenMenu(pk, uiNpcId);
        return;
    }

    // 1) Find the ViewRow first (NPCViewInfo.shn -> action tag + args).
    std::string kAction, kArg0, kArg1;
    const NPCDialogTables::ViewRow* pkV =
        NPCDialogTables::Get().FindView(uiViewInfoId);
    if (pkV) {
        kAction = pkV->kActionTag; kArg0 = pkV->kArg0; kArg1 = pkV->kArg1;
    } else {
        // Fall back to NPCAction.txt rows for this NPC's menu page.
        uint8 page = NPCManager::Get().MenuOf(uiNpcId);
        std::vector<const NPCActionTable::ActionRow*> kRows;
        NPCActionTable::Get().ActionsForMenu(page, kRows);
        for (size_t i = 0; i < kRows.size(); ++i) {
            if (kRows[i]->uiViewInfoId == uiViewInfoId) {
                kAction = kRows[i]->kAction;
                kArg0   = kRows[i]->kArg0;
                kArg1   = kRows[i]->kArg1;
                break;
            }
        }
    }

    SHINELOG_DEBUG("MenuPick char=%u npc=%u view=%u action=%s arg0=%s arg1=%s",
                   (uint32)pk->GetCharID(), uiNpcId, uiViewInfoId,
                   kAction.c_str(), kArg0.c_str(), kArg1.c_str());

    // 2) Route by action tag. Tags follow the strings used in NPCAction.txt
    //    and the per-button "Action" column in NPCViewInfo.shn.
    if (kAction == "Talk" || kAction == "Dialog") {
        // Arg0 is a numeric DialogID; render the chained sub-dialog.
        uint32 nextDialog = (uint32)strtoul(kArg0.c_str(), NULL, 10);
        if (nextDialog == 0) {
            SendPickAck(pk, uiNpcId, uiViewInfoId, 0, std::string());
            return;
        }
        SendDialog(pk, uiNpcId, nextDialog);
        SendPickAck(pk, uiNpcId, uiViewInfoId, 1, std::string());
        return;
    }
    if (kAction == "Trade" || kAction == "Shop" || kAction == "Buy") {
        OpenShop(pk, uiNpcId);
        SendPickAck(pk, uiNpcId, uiViewInfoId, 1, std::string());
        return;
    }
    if (kAction == "Quest") {
        // Arg0 = quest handle (numeric).
        uint32 questId = (uint32)strtoul(kArg0.c_str(), NULL, 10);
        bool ok = false;
        if (questId) {
            // Fire the begin/finish path through CharQuest. Begin returns
            // false if the player already has it active (in which case we
            // try to finish it).
            if (pk->Quest().State(questId) == QS_ACTIVE) {
                ok = pk->Quest().Finish(questId);
            } else {
                ok = pk->Quest().Begin(questId);
            }
        }
        SendPickAck(pk, uiNpcId, uiViewInfoId, ok ? 1 : 0, std::string());
        return;
    }
    if (kAction == "Mover" || kAction == "Warp" || kAction == "Teleport") {
        // The client owns the actual map-load; we just acknowledge so the
        // dialog box closes. Arg0/Arg1 = destination map / coords; the
        // existing MoverList handler in the client will dispatch the
        // teleport from this ack.
        SendPickAck(pk, uiNpcId, uiViewInfoId, 1, kArg0);
        return;
    }
    if (kAction == "Promote" || kAction == "JobChange") {
        SendPickAck(pk, uiNpcId, uiViewInfoId, 1, kArg0);
        return;
    }
    // Enchanter NPCs. Action tags observed in NpcDialogData.shn /
    // NPCViewInfo.shn:
    //   "Upgrade"           -> generic +N enchant (any equip type)
    //   "Enchant"           -> alias used by some Roumen NPCs
    //   "Refine"            -> alias used by accessory upgraders
    //   "EquipmentUpgrade"  -> the canonical NA2016 button text
    // Arg0 narrows to a specific equip kind: "weapon"/"armor"/"accessory".
    // The session is single-shot -- the actual roll happens on
    // NC_ITEM_UPGRADE_REQ which clears the session.
    if (kAction == "Upgrade" || kAction == "Enchant" ||
        kAction == "Refine"  || kAction == "EquipmentUpgrade") {
        uint8 uiKind = 0;
        if      (kArg0 == "weapon"    || kArg0 == "1") uiKind = 1;
        else if (kArg0 == "armor"     || kArg0 == "2") uiKind = 2;
        else if (kArg0 == "accessory" || kArg0 == "3") uiKind = 3;
        ItemUpgrade::OpenSession(pk, uiNpcId, uiKind);

        // Tell the client to open the enchanter UI. Body matches the
        // documented NA2016 wire shape:
        //   uint32 npcId
        //   uint8  kind         (0/1/2/3)
        //   uint8  allowsLuck   (1 if Luck Stones can be consumed)
        //
        // PROVISIONAL_BODY: the (npcId, kind, allowsLuck) ordering is
        // provisional. The NA2016 client likely expects a specific
        // field set/order (possibly including a list of allowed item-
        // class filters). Reconcile against a real packet capture
        // before treating this as canonical.
        PacketBuffer body;
        body.WriteU32(uiNpcId);
        body.WriteU8 (uiKind);
        body.WriteU8 (1);     // every enchanter accepts Luck Stones
        GPacket kPkt; kPkt.SetOpcode(NC_ITEM_UPGRADE_OPEN_CMD);
        kPkt.Body().WriteBytes(body.Data(), body.Size());
        if (pk->GetSession()) pk->GetSession()->SendPacket(kPkt);

        SendPickAck(pk, uiNpcId, uiViewInfoId, 1, std::string());
        SHINELOG_INFO("Enchanter session OPEN cid=%u npc=%u kind=%u",
                      (uint32)pk->GetCharID(), uiNpcId, (uint32)uiKind);
        return;
    }
    if (kAction == "Save" || kAction == "SavePoint" || kAction == "Recall") {
        // No state mutation server-side; the save-point system writes on
        // the next CharDB sync. Acknowledge so the UI proceeds.
        SendPickAck(pk, uiNpcId, uiViewInfoId, 1, std::string());
        return;
    }
    if (kAction == "Close" || kAction == "Cancel") {
        SendPickAck(pk, uiNpcId, uiViewInfoId, 1, std::string());
        return;
    }

    // Unknown action tag -- log + ack 0 so the client can fall back to its
    // legacy local handler.
    SHINELOG_WARN("MenuPick: unhandled action='%s' on npc=%u view=%u",
                  kAction.c_str(), uiNpcId, uiViewInfoId);
    SendPickAck(pk, uiNpcId, uiViewInfoId, 0, std::string());
}

// -----------------------------------------------------------------------------
//  NC_NPC_SHOP_BUY_REQ. Validates SKU via NPCItemList, debits gold via
//  SellItemManager, then synthesises a ShineItem and inserts it into the
//  player's inventory. Pre-rolled stacks of size > MaxLot are not split
//  here -- the client clamps qty before the request lands.
//  Reply: NC_NPC_SHOP_BUY_ACK [ uint8 result, uint32 inx, uint16 qty,
//                               int64  newGold ]
// -----------------------------------------------------------------------------
void ServerMenuActor::HandleBuy(ShinePlayer* pk, uint32 uiNpcId, uint32 uiInxName, uint16 uiQty) {
    uint8 result = 0;
    if (pk && uiQty != 0 && SellItemManager::BuyFromNpc(pk, uiNpcId, uiInxName, uiQty)) {
        // Money has already been debited. Insert the item record.
        const ItemInfoRow* pkI = ItemTables::Get().FindItem(uiInxName);
        if (pkI) {
            ShineItem si;
            si.uiItemId = 0;                   // assigned by Inventory::Add
            si.uiInxName = (ItemID)pkI->uiID;
            si.uiSlot    = 0;                  // first free slot
            si.uiQty     = uiQty;
            si.uiEndure  = 0;
            si.uiEnchant = 0;
            si.bEquipped = 0;
            si.kItemIndex = pkI->kInxName;
            for (int j = 0; j < 5; ++j) si.aRandomOption[j] = 0;
            if (pk->Inv().Add(si)) result = 1;
        }
    }
    PacketBuffer body;
    body.WriteU8 (result);
    body.WriteU32(uiInxName);
    body.WriteU16(uiQty);
    body.WriteU64(pk ? pk->GetMoney() : 0);
    GPacket kPkt; kPkt.SetOpcode(NC_NPC_SHOP_BUY_ACK);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    if (pk && pk->GetSession()) pk->GetSession()->SendPacket(kPkt);
}

// -----------------------------------------------------------------------------
//  NC_NPC_SHOP_SELL_REQ. The client identifies the stack by inventory
//  slot. The sell price is read from ItemInfo.uiSellPrice; partial-stack
//  sells are clamped to the actual stack size.
// -----------------------------------------------------------------------------
void ServerMenuActor::HandleSell(ShinePlayer* pk, uint32 uiNpcId, uint16 uiInvSlot, uint16 uiQty) {
    (void)uiNpcId;
    uint8 result = 0;
    if (pk && uiQty != 0) {
        const std::vector<ShineItem>& kAll = pk->Inv().All();
        const ShineItem* pkS = NULL;
        for (size_t i = 0; i < kAll.size(); ++i) {
            if (kAll[i].uiSlot == uiInvSlot) { pkS = &kAll[i]; break; }
        }
        if (pkS && ItemAuthority::CanSell(*pkS)) {
            uint16 actual = (uiQty > pkS->uiQty) ? pkS->uiQty : uiQty;
            const ItemInfoRow* pkI = ItemTables::Get().FindItem((uint32)pkS->uiInxName);
            if (pkI) {
                int64 gain = (int64)pkI->uiSellPrice * (int64)actual;
                if (pk->Inv().Remove(pkS->uiItemId)) {
                    pk->AddMoney(gain);
                    result = 1;
                }
            }
        }
    }
    PacketBuffer body;
    body.WriteU8 (result);
    body.WriteU16(uiInvSlot);
    body.WriteU16(uiQty);
    body.WriteU64(pk ? pk->GetMoney() : 0);
    GPacket kPkt; kPkt.SetOpcode(NC_NPC_SHOP_SELL_ACK);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    if (pk && pk->GetSession()) pk->GetSession()->SendPacket(kPkt);
}

bool SellItemManager::BuyFromNpc(ShinePlayer* pk, uint32 uiNpcId, uint32 uiInx, uint16 uiQty) {
    if (!pk || uiQty == 0) return false;
    // Confirm the NPC actually offers this item.
    std::vector<NPCMenuItem> kList;
    NPCItemList::GetForShop(uiNpcId, kList);
    bool ok = false; int64 iPrice = 0;
    for (size_t i = 0; i < kList.size(); ++i) {
        if (kList[i].uiInxName == uiInx) { ok = true; iPrice = kList[i].iPrice; break; }
    }
    if (!ok) return false;
    int64 iTotal = iPrice * (int64)uiQty;
    if ((int64)pk->GetMoney() < iTotal) return false;
    pk->AddMoney(-iTotal);
    // Inventory insert is owned by the inventory subsystem; the NPC menu
    // dispatches the resolved (uiInxName, qty) pair into it via the
    // existing CMSG_BUY_FROM_NPC handler. We've already debited the gold
    // and validated the SKU here.
    return true;
}

void NpcScheduleServer::Tick() {
    // Hour-of-week granular visibility toggle. Every NPC registered in
    // NPCManager is matched against `NpcScheduleTable::IsActive`; if the
    // active state has flipped since the last tick we surface a log line
    // (the per-field visibility broadcast lives in the spawn pass and
    // is keyed by the same active-flag, so the Field walk owns the
    // wire-side update). This implementation is the cadence anchor;
    // adjust the polling cadence by changing how often the parent tick
    // calls into NpcScheduleServer (default: once per second).
    SYSTEMTIME st; GetLocalTime(&st);
    uint32 hourOfWeek = (uint32)st.wDayOfWeek * 24u + (uint32)st.wHour;
    static std::map<std::string, bool> s_kLastActive;
    NPCManager& nm = NPCManager::Get();
    const std::map<uint32, std::string>& kKeys = nm.NpcKeys();
    for (std::map<uint32, std::string>::const_iterator it = kKeys.begin();
         it != kKeys.end(); ++it) {
        bool bNow = NpcScheduleTable::Get().IsActive(it->second, hourOfWeek);
        std::map<std::string, bool>::iterator pit = s_kLastActive.find(it->second);
        bool bWas = (pit != s_kLastActive.end()) ? pit->second : true;
        if (bNow != bWas) {
            s_kLastActive[it->second] = bNow;
            SHINELOG_INFO("NPC schedule: '%s' -> %s",
                          it->second.c_str(), bNow ? "ACTIVE" : "INACTIVE");
        }
    }
}

} // namespace fiesta
