# Original Naming and Function Reuse Guide

Emergent should not invent clean-room names first. Use the PDB-derived names as the source-name vocabulary.

## Required CSVs

- `function_names/full_function_symbols_filtered.csv` — 92563 filtered function/symbol candidates.
- `function_names/protocol_handlers_and_packet_symbols.csv` — 31445 packet/protocol-related names.
- `function_names/original_object_modules_from_pdb.csv` — 621 object/source modules recovered from PDB path strings.
- `function_names/function_names_by_system.md` — readable grouped subset.

## Naming rules

1. If a PDB symbol exists, reuse it or create a wrapper with the original name visible.
2. Preserve prefixes:
   - `sp_NC_*` = ShinePlayer/client request handler in Zone.
   - `gds_NC_*` = GameDBSession DB ack/result handler.
   - `WM*Session`, `Login*Session`, `PF_*` = service routing/session/parser families.
   - `UseEffect::*` = item-use behavior classes.
   - `Roe_*`, `normalpyRoe*`, `normalmaRoe*` = combat Rule-of-Engagement pipeline.
   - `SHND::CommonData`/`DataBox` = table/index loader layer.
3. Keep original module family names: `ShineItemEquip`, `ShineItemUse`, `MobBrain`, `KingdomQuest`, `InstanceDungeon`, `HolyPromise`, `GuildTournamentSystem`, etc.
4. For unknown packet hex values, name handlers by `NC_*` symbol until capture fills hex.
5. Do not collapse feature systems into giant god classes. Use original object module boundaries from the PDB.

## High-value original module families

- Core/shared: `DataReader`, `ShnDataFileCheckSum`, `GPacket`, `PacketEncrypt`, `IOCP`, `Socket_IOCP`, `WinService`.
- Zone object: `ShineObject`, `ShinePlayerObj`, `ShineObjectMob`, `ShineObjectPet`, `ShineObjectMover`, `BattleObject`.
- Combat: `RuleOfEngagement`, `Battle`, `DamageByAngle`, `DamageBySoul`, `LevelGapDamageTable`, `AttackRhythm`.
- Items: `ShineItem*`, `ItemAction*`, `ItemDrop*`, `ItemMall`, `ChargedItem*`, `SetItemData`, `ItemRandomOption`.
- Script/content: `LuaScript`, `PineScript*`, `QuestFramework`, `QuestEvent`, `ScenarioBookShelf`.
- Social/world: `Guild*`, `GuildAcademy*`, `GuildTournament*`, `HolyPromise*`, `Party*`, `RaidSystem`, `PartyFinderServer`.
- Events: `KingdomQuest`, `InstanceDungeon`, `KQServer`, `MatchInstanceDungeonServer`.
