# Honest Disclosure: Stubs, Guesses, and Verify Markers

This document is for the next reviewer (codex audit pass, opus static
debug, sonnet runtime debug, opus function verification). It catalogs
everything I know to be incomplete, invented, or stubbed in the Zone
server tree so you don't have to discover it the hard way.

I'd rather you find this list and triage it than spend cycles
chasing ghosts in a 350-file directory.

---

## 1. Dead anonymous-namespace stub TUs

The following 27 `.cpp` files contain a class wrapped in
`namespace fiesta { namespace { ... } }` whose symbols are NEVER
referenced from anywhere else in the tree. They exist to occupy a
project-file slot mirroring the NA2016 PDB layout, not to provide
working behaviour:

```
AbnormalState.cpp                 -- aggregate already in AbState.h
AbnormalStateDictionary.cpp       -- string/id mapping, no callers
AbnormalStateShelter.cpp          -- shelter-list, no callers
AttackRhythm.cpp                  -- per-class swing cadence, no callers
BRAccUpgradeDataBox.cpp           -- accessory-upgrade table, no callers
BoothManager.cpp                  -- player shop booths, no callers
CharacterSkill.cpp                -- skill loadout, no callers
GuildAcademy.cpp                  -- guild academy, no callers
KQContribute.cpp                  -- kingdom-quest contribution, no callers
KQRewardDataBox.cpp               -- kq reward table, no callers
MIDRewardDataBox.cpp              -- monster-island reward table, no callers
MapDataBox.cpp                    -- duplicate of MapField.h struct, no callers
MinimonDataBox.cpp                -- minimon table, no callers
MoverDataBox.cpp                  -- mover/teleport table, no callers
MoverUpgradeData.cpp              -- mover upgrade table, no callers
MultiHitTable.cpp                 -- multi-hit lookup, no callers
MysteryVaultTable.cpp             -- mystery vault table, no callers
NearScan.cpp                      -- proximity scan, partially used
PartyContainer.cpp                -- party state, no callers
PetRelatedSkill.cpp               -- pet skill table, no callers
RuleOfEngagement.cpp              -- PvP rules, no callers
SellItemManager.cpp               -- per-NPC sell stock, no callers
SetItemData.cpp                   -- equipment set bonuses, no callers
SkillDataBox.cpp                  -- skill table, no callers
SpamerPenaltyDataBox.cpp          -- spam-penalty table, no callers
SubAbstatePriority.cpp            -- abstate priority, no callers
TargetAnalyser.cpp                -- target picker, no callers
```

The reason they're in anonymous namespaces is to suppress LNK2005
collisions with the same class names that already appear inside
aggregate headers (`ItemSystems.h`, `MapField.h`, etc.). When you
flesh one of these out, lift it OUT of the anonymous namespace and
delete the duplicate declaration in the matching aggregate header.

`MapBlockInformation.cpp` is a no-op TU after the .shbd refactor --
the canonical struct moved to `MapField.h`. Either delete the file or
leave it; it emits no symbols either way.

## 2. Invented constants (I had no source data; placeholder values)

* `Server/Zone/LiveOpsBoosts.h` -- `kGMEvent_LuckyHour=1001`,
  `kGMEvent_DoubleExp=1002`, `kGMEvent_DoubleDrop=1003`,
  `kGMEvent_GoldenHour=1004`. Real GMEvent.shn EventNo values are NOT
  catalogued in this tree. Symptom of mismatch: WM windows fire but
  zones only emit a generic "GM event N has begun" banner instead of
  applying boosts.
* `Server/Zone/MapField.h` -- `kMapBlockCellSize = 16`. World-to-cell
  ratio inferred from a doc comment. Wrong value = players phase
  through walls or get stuck in walkable terrain.
* `Server/Zone/BattleTunables.h` -- `kFailPenaltyAtLevel[]`,
  `kUpgradeCritRollMax`, `kDropRateGlobalScalerX1k`. Header itself
  admits these need community-theorycraft verification.
* `Server/Zone/DeathReviveSystem.cpp` -- `ExpLossOnTownRevive` curve
  is a placeholder.

## 3. Invented packet shapes (no NA2016 capture available)

These three opcodes have body layouts I made up to match what the
existing zone-side handlers expected. They WILL mismatch a real client.

* `NC_ITEM_UPGRADE_OPEN_CMD` (NC_FAMILY_ITEM + 0x13)
  Current: `{ uint32 npcId, uint8 kind, uint8 allowsLuck }`
  Sent from `NPCSystem::HandlePick` on an "Upgrade"/"Enchant" button.
* `NC_NPC_MENU_PICK_ACK` (existing opcode, body shape not from capture)
  Current: `{ uint32 npcId, uint32 viewInfoId, uint8 result, string msg }`
  Used for every Promote/Save/Recall/etc. response.
* `NC_INTER_GMEVENT_TRIGGER_REQ` (NC_FAMILY_INTERSVR + 0x15)
  Current: `{ uint32 eventNo, uint32 durationSec, uint8 action }`
  Internal Zone -> WM admin push. Doesn't need to match a real client
  since both ends are this server, but the opcode number itself
  could collide with an unmapped original-server inter opcode.

## 4. Schema lookups I haven't proven exist

* `Server/Zone/GroupTables.cpp` reads `Money` from MobInfo.shn into
  `MobInfoRow.uiMoney`. ShnGetU32 returns 0 if the column is missing,
  which silently disables the gold-drop path. Verify the column name
  matches the actual NA2016 MobInfo.shn header.
* `Server/Zone/GMEventManager.cpp` reads `EventNo`, `StartTime`,
  `EndTime` from GMEvent.shn. Same risk -- a missing column makes
  every event look like it has a 1-hour duration.

## 5. Functional gaps (will compile + run, but the feature is wrong)

* `ItemUpgrade::Try` does NOT consume the `UpResource` material from
  the player's inventory. The original game removes one upgrade stone
  per attempt regardless of outcome. Players currently get unlimited
  free attempts. Marked with VERIFY in the code.
* `ItemUpgrade::Try` IGNORES `bUseLuckStone` (logs a warning and
  treats it as false) because the Luck-Stone item id binding isn't
  configured. Marked with VERIFY in the code.
* `WMClient::OnTakeItem` does not actually delete the item from the
  player's Inventory; it only logs. The OPTool DB-side delete works
  through a separate path. Marked in the existing comment.
* `WorldManager::DailyQuestTimer` and friends fan out via
  NC_INTER_BROADCAST_CMD kind=1 but the Zone side does not yet handle
  kind=1 (only kind=2 GM events are wired). Daily resets are dropped
  on the floor.

## 6. Things I'd push back on if I were reviewing

* The 350+ Zone .cpp files list mirrors the PDB, but a lot of those
  files are 5 lines long. Suggest collapsing the dead-stub batch
  into 5-10 thematic aggregate files (one per subsystem) once the
  build is stable -- cuts compile time and link surface meaningfully.
* `Server/Zone/AmpersandCommands.cpp` has a 154-entry default-handler
  table where every command logs and returns true. Until a command
  has a real body, those slots advertise capabilities the server
  doesn't actually deliver. Consider dropping the unimplemented
  entries from the public ampersand list rather than acking them.
* `LiveOpsBoosts::Get()` is a process-wide singleton; if the project
  ever runs multiple Zones in one process (unlikely but technically
  possible per the architecture), boosts would leak across them. The
  proper home is `ZoneServer` instance state.

## 7. Things I'm pretty confident in

* SHN/DAT readers (`Server/DataReader/`) -- typed schemas match the
  observed headers. The .shbd reader was tightened to fail-loud on
  geometry mismatches in this pass.
* `LiveOpsBoosts` core scaler logic -- math is correct, expiry tick
  works, applies cleanly in Battle.cpp and ItemSystems.cpp.
* The GMEventManager_Zone broadcast plumbing -- the wire path is
  consistent end-to-end (Zone -> WM -> all Zones -> LiveOpsBoosts).
* The map-block load failure path is now hard-failure instead of
  silent-corruption.
* Anti-cheat session for ItemUpgrade -- single-shot session keyed by
  CharID, cleared on disconnect, refuses raw NC_ITEM_UPGRADE_REQ
  packets without an open NPC click.

---

If any of the items in sections 1-5 are blockers for your audit pass,
flag them and I'll wire up the data binding (or remove the feature) in
the next round.
