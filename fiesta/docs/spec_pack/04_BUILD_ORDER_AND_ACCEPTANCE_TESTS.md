# Build Order and Acceptance Tests

This order prevents Emergent from writing gameplay features before the original server framework exists.

## L0 Proof/Ingestion

**Scope:** Import this pack, PDB symbols, current recovered source, docs, data manifests. Generate per-system pages.

**Acceptance:** Every system has owner, data files, original names, proof level, and required function surface. Unknown formulas are marked `VERIFY/TUNE` inside the function body.

## L1 Foundation

**Scope:** WinService/process bootstrap, logging, crash dumps, config, IOCP sockets, packet buffer, random, time, token/cipher.

**Acceptance:** Services start/shutdown cleanly and can echo/test packets.

## L2 Data Layer

**Scope:** SHN/TXT guarded loader, checksums, registry, typed rows, CommonData/DataBox indexes.

**Acceptance:** Loads all safe non-quest data; quest files blocked by guard unless explicitly supported.

## L3 Database Layer

**Scope:** Restore .bak schema/procs, DB connectors/sessions, DB packet processors, state save boundaries.

**Acceptance:** Can load account/character/inventory/skills with no gameplay yet.

## L4 Login/WM/Zone Skeleton

**Scope:** Login handshake, WM token/world/char routing, Zone login/map sync, client session object.

**Acceptance:** Client can login, select char, enter map and idle.

## L5 Character/Stats/Items Basic

**Scope:** Character stats, inventory/equip/use minimal, item restrictions, stat packet builder.

**Acceptance:** Equip/unequip updates DB and 1035/stat packets match capture.

## L6 Map/Movement/Entity

**Scope:** Map manager, field/link/portal, entity visibility, movement and brief info.

**Acceptance:** Movement/portal/NPC approach packet clusters match capture.

## L7 Combat/Skill/AbState

**Scope:** Roe, hit/miss/crit, skills, cooldown, buffs/debuffs, death/respawn, drops/EXP.

**Acceptance:** Mob fight, skill, death, level-up handlers/functions exist with original-style names and local tunable formulas. Exact numbers can be tuned later by editing those functions.

## L8 NPC/Quest/PineScript/Lua

**Scope:** NPC menus, shops, quest framework, PineScript, Lua C bindings, scenario/job change.

**Acceptance:** Job change can be repeated with DB rollback and matches packets/DB writes.

## L9 Social/World Features

**Scope:** Party, raid, guild, academy, wedding/HolyPromise, master/apprentice, friends/chat.

**Acceptance:** Core social flows produce expected packets/DB writes.

## L10 Events/Instances/Gambling/Markets

**Scope:** KQ, MID/instances, auction/market, booths, slot machine/dice, card collection, minimon/pets, mini-house/movers.

**Acceptance:** Dungeon/static-damage/event/market handlers exist with original-style names. Exact reward/static-damage constants can be tuned later inside the owning functions.

## L11 Admin/Optool/GM/Security

**Scope:** Ampersand commands, optool sessions, prison, XTrap/version, spam/bad-name filters, logs/reports.

**Acceptance:** Admin commands gated by AdminLvSet and DB/auth; no accidental unrestricted commands.

## Do not mark complete until

- Original PDB name mapping exists for every implemented handler where available.
- Packet opcode/struct has capture evidence or is explicitly `placeholder`; formula constants may be `VERIFY/TUNE` but the owning function must exist.
- DB write lines are matched for state-changing systems.
- No quest SHN was bulk-parsed or rewritten by unsafe tools.

