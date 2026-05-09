# Pass 1.22 -- Server-Authoritative NPC Menu Loop

## Goal
Close the inbound half of the NPC click flow so that the server fully drives
both ends of the dialog box (talk, trade, quest, mover, promote, save).
Previously only the *outbound* `NC_NPC_MENU_OPEN_CMD` packet was being built;
the client had nowhere to send button presses to.

## Files Touched
- `Server/Zone/NPCSystem.h`   -- expanded `ServerMenuActor` API.
- `Server/Zone/NPCSystem.cpp` -- new `SendDialog`, `OpenShop`, `HandlePick`,
                                  `HandleBuy`, `HandleSell`, `SendPickAck`.
- `Server/Zone/ZoneHandlers.cpp` -- registered three new opcodes.

## Wire Protocol (in / out)

### Outbound
`NC_NPC_MENU_OPEN_CMD` (existing + reused for chained sub-dialogs):
```
uint32 npcId
uint32 dialogID
pstr   dialogTextKey
uint8  buttonCount
{ uint32 viewInfoId
  pstr   labelKey
  uint32 iconId
  pstr   actionTag
  pstr   arg0
  pstr   arg1 } * buttonCount
```
`NC_NPC_MENU_PICK_ACK`:
```
uint32 npcId
uint32 viewInfoId
uint8  result        (1 ok / 0 fail)
pstr   message
```
`NC_NPC_SHOP_OPEN_CMD`:
```
uint32 npcId
uint16 itemCount
{ uint32 inxName, int64 buyPrice } * itemCount
```
`NC_NPC_SHOP_BUY_ACK`:
```
uint8  result, uint32 inxName, uint16 qty, int64 newGold
```
`NC_NPC_SHOP_SELL_ACK`:
```
uint8  result, uint16 invSlot, uint16 qty, int64 newGold
```

### Inbound
`NC_NPC_MENU_PICK_REQ` -- `[ uint32 npcId, uint32 viewInfoId ]`
- `viewInfoId == 0` is the synthetic *root* click (client just opened the
  talk box); the server runs `OpenMenu` and bumps "Meeting" quest counters
  via `QuestProgress::OnNpcTalked`.
- Otherwise the ViewRow is resolved from `NPCViewInfo.shn` (with fallback to
  the matching `NPCAction.txt` row) and routed by its action tag:
    - `Talk` / `Dialog`   -> `SendDialog(arg0)` (chained DialogID)
    - `Trade` / `Shop` / `Buy` -> `OpenShop`
    - `Quest`             -> `CharQuest::Begin` or `Finish` (arg0 = quest id)
    - `Mover` / `Warp` / `Teleport` -> ack only (client owns map-load)
    - `Promote` / `JobChange`       -> ack only (PromoteSystem owns the flow)
    - `Save` / `Recall`             -> ack only (handled on next CharDB sync)
    - `Close` / `Cancel`            -> ack only

`NC_NPC_SHOP_BUY_REQ`  -- `[ uint32 npcId, uint32 inxName, uint16 qty ]`
`NC_NPC_SHOP_SELL_REQ` -- `[ uint32 npcId, uint16 invSlot, uint16 qty ]`

## Buy Path
`SellItemManager::BuyFromNpc` validates the SKU against
`NPCItemList::GetForShop` (driven by the per-NPC `NPCItemList/<MobName>.txt`
walkthrough wired in pass 1.20) and debits gold. On success the menu actor
synthesises a `ShineItem` from `ItemTables::FindItem(inxName)` and inserts
it via `Inventory::Add`. The `kInxName` (33-char string id) is preserved
on the stack so subsequent stat composition / equip checks have it.

## Sell Path
The slot-resolved stack is read from `Inventory::All()`, the sell price is
sourced from `ItemInfoRow::uiSellPrice`, the qty is clamped to the actual
stack size, and `Inventory::Remove` is invoked with the original
`uiItemId`. Money is added back via `ShinePlayer::AddMoney`.

## Quest Path
Only the high-level Begin / Finish lifecycle is wired here. The detailed
goal-counter (Hunting / Looting / Meeting / Produce) flow runs on the
existing `QuestRuntime` and `QuestProgress` singletons -- this menu actor
just decides which entry point to fire based on the player's current
`QuestState`.

## Open Items
- The "Mover" action tag currently acks-and-returns; if/when the client
  expects the server to drive the actual `NC_MAP_LINKSAME_REQ` / map-load,
  hook the `MoverList` table here.
- Quest dialogs that need pre-conditions beyond level (item-have, quest-
  have) still need to filter the button list; the gate currently only
  honours `NPCCondition.kConditionA == "Level"`.
