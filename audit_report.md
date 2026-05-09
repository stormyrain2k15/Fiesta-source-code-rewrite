# Fiesta Source Rewrite — Static Code Audit

**Scope:** static audit of `Fiesta-source-code-rewrite.-main.zip` only. I did not run the server, did not parse quest SHNs, and did not attempt a VS2010 build inside this environment.

## Verdict

This is a broad **pass-1 source map / function-surface rewrite**, not a compile-ready or client-test-ready server yet.

It does a good job putting many original-ish systems, modules, and names into the tree, but several files are still facade/stub coverage and there are hard compile/link blockers. The current repo should be treated as a strong scaffold for Emergent to keep expanding, not as something ready to connect the existing Fiesta client to yet.

## What is good

- Large coverage surface: `1,795` C++ header/source files under `fiesta`, about `338,998` source lines.
- Project-file coverage is clean: parsed `.vcxproj` references showed `0` missing referenced source/header files and no obvious source files missing from projects under `Server`, `Client`, or `Lua`.
- VS2010-style canary scan did not find obvious modern C++ constructs such as `nullptr`, range-for, `enum class`, `constexpr`, `decltype`, or `static_assert` in the expected source paths.
- The repo includes an honest disclosure document that correctly admits many placeholders, invented packet shapes, unproven schema lookups, and functional gaps.
- The design direction is mostly correct: server first, original-ish module names, SHN/DataReader layer, protocol enums, Zone/WM/Login/DataServer split, and many system names preserved.

## Hard compile/link blockers

### 1. Invalid Lua include syntax in `AIScript.cpp`

`Server/Zone/AIScript/AIScript.cpp:5` has preprocessor directives inside a one-line `extern "C"` block:

```cpp
extern "C" { #include "lua.h"  #include "lualib.h"  #include "lauxlib.h" }
```

That is invalid C/C++ preprocessing. It must be split onto separate lines:

```cpp
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
```

### 2. `RegisterZoneLuaAPI(lua_State*)` is referenced but not defined

Referenced by:

- `Server/Zone/AIScript/AIScript.cpp`
- `Server/Zone/LuaScript/LuaScript.cpp`

Available functions are instead:

- `RegisterAllLuaAPIs(lua_State*)`
- `RegisterLuaEnums(lua_State*)`
- `LuaRuntime::RegisterCBindings()`

This will fail link unless a wrapper is added or Zone scripts are routed through `LuaRuntime`.

Suggested original-style wrapper:

```cpp
void RegisterZoneLuaAPI(lua_State* L) {
    RegisterAllLuaAPIs(L);
    RegisterLuaEnums(L);
    // install real Zone C bindings here, not only stubs
}
```

### 3. Bad checksum include and wrong MD5 API

`Server/Zone/ShnDataFileCheckSum.cpp` includes a non-existent path:

```cpp
#include "../Shared/Md5/MD5Checksum.h"
```

Actual path is:

```cpp
#include "../Shared/MD5Checksum.h"
```

It also calls `MD5Checksum::HexOf(s)`, but the actual API uses `Compute(...)` and `ToHex(...)`. There is already a better implementation in `Server/DataReader/ShnDataFileCheckSum.cpp`. Prefer deleting or excluding the bad Zone duplicate, or forward the Zone code to the DataReader checksum class.

### 4. Undefined opcode names are used in live code

`Server/Common/NETCOMMAND.h` does not define several `NC_*` names used by code.

Compile-breaking examples:

- `NC_USER_CLIENT_VERSION_CHECK_REQ` in `Server/Login/LoginClientSession.cpp`
- `NC_ACT_MOVERUN_CMD` in `Server/Zone/ZoneHandlers.cpp`
- `NC_ACT_MOVEWALK_CMD` in `Server/Zone/ZoneHandlers.cpp`
- `NC_ACT_STOP_CMD` in `Server/Zone/ZoneHandlers.cpp`
- `NC_BAT_NORMALATTACK_CMD` in `Server/Zone/ZoneHandlers.cpp`
- `NC_USER_WORLD_SELECT_REQ` / `NC_USER_WORLD_SELECT_ACK` in `Client/ResSystem/UIResourceTables.cpp`
- `NC_QUEST_BEGIN_REQ`, `NC_PARTY_REQ`, `NC_ATTEND_GET_CMD` in `Client/ResSystem/UIResourceTables.cpp`

Likely mappings already present in `NETCOMMAND.h`:

- `NC_ACT_MOVERUN_CMD` / `NC_ACT_MOVEWALK_CMD` → probably `NC_MAP_MOVE_REQ` plus move-kind body value, or separate aliases if confirmed by PDB/capture.
- `NC_ACT_STOP_CMD` → `NC_MAP_STOP_MOVE_REQ`
- `NC_BAT_NORMALATTACK_CMD` → `NC_BAT_ATTACK_REQ`
- `NC_USER_WORLD_SELECT_REQ/ACK` → `NC_USER_WORLDSELECT_REQ/ACK`
- `NC_QUEST_BEGIN_REQ` → `NC_QUEST_START_REQ`
- `NC_PARTY_REQ` → one of `NC_PARTY_CREATE_REQ`, `NC_PARTY_JOIN_REQ`, etc., not a generic party req.

Do not randomly add aliases unless they match captured/PDB names. Put comments as `CONFIRMED`, `PDB_NAME_ONLY`, or `PROVISIONAL`.

### 5. `SQLP.cpp` uses varargs without required include and uses VS2010-risky formatting

`Server/DataServer/Common/SQLP.cpp` uses `va_list`, `va_start`, and `va_end` but only includes `<stdio.h>`. Add `<stdarg.h>`.

It also uses `vsnprintf`. VS2010 compatibility is questionable; prefer `_vsnprintf_s` or a small wrapper that is explicitly VS2010-safe.

## Critical functional gaps before existing-client testing

### 1. Zone combat targets only players, not mobs

`Server/Zone/ZoneHandlers.cpp` normal attack and skill-use lookup targets only in `ZoneServer::Players()`:

```cpp
std::map<Handle, ShinePlayer*>::const_iterator it = ZoneServer::Get().Players().find(h);
```

That means PvE attacks/skills against mobs will fail or do nothing. A real Zone needs a unified object registry:

```text
handle -> ShineObject
  players
  mobs
  NPCs
  drops
  movers/pets/minimon
  scripted objects
```

### 2. Skill cooldowns are global/static, not per character

`Skill::TryUse` uses:

```cpp
static CharacterSkill s_kDummy;
```

That makes cooldown/skill readiness shared globally instead of per player. It also subtracts HP/SP before the cooldown check, so a failed cooldown can still drain resources.

Fix order should be:

```text
validate target/range/state/class/prereqs
validate cooldown
validate resources
spend resources
execute cast/damage/abstate
start cooldown on the player's CharacterSkill object
```

### 3. Roe/combat naming exists, but not close enough to original evidence

`Battle.cpp` has `RuleOfEngagement::CalcDamage`, but current math is mostly `ATK - DEF` with invented scalers. Earlier evidence points to original functions like:

```text
Roe_calcdamage
Roe_getattack
Roe_defendpower
Roe_hitrate
Roe_criticalrate
Roe_isdamageincrease
normalpyRoe_calc / normalpyRoe_damage
normalmaRoe_calc / normalmaRoe_damage
```

This is not a blocker for exact numeric tuning, but the function surface should be reshaped to preserve those original entry points. Unknown constants can stay local in those functions with `VERIFY/TUNE` comments.

### 4. Lua API is mostly stub surface

`Lua/LuaAPI.cpp` registers a large Lua API surface, but most functions return `0` only. `Lua/LuaCBindings.cpp` overrides only five functions:

```text
cDamaged
cStaticDamage
cSetAbstate
cLinkTo
cFinishKey
```

That means KQ, instance, scripted boss, NPC, class-change, reward, object, and timer scripts will mostly no-op. Static damage and abstate are also incomplete: `cStaticDamage` subtracts HP directly and skips normal death/packet pipelines; `cSetAbstate` uses a static bus instead of per-object/per-player state.

### 5. Market/trade/auction/booth systems are not gameplay-complete

The repo has the names, but many functions are in-memory, return true, or omit money/item transfer and DB ownership. This is fine for pass 1, but not client-ready.

### 6. Item upgrade is admitted incomplete

`HONEST_DISCLOSURE.md` correctly states:

- upgrade resource is not consumed
- luck stone is ignored
- invented packet shape exists for `NC_ITEM_UPGRADE_OPEN_CMD`

Keep this in the blocker list before exposing item upgrade to a real client.

### 7. Login/world select is still provisional

`LoginClientSession.cpp` returns hardcoded WM endpoint:

```cpp
ack.WriteString("127.0.0.1");
ack.WriteU16(28000);
```

That needs to come from `ServerInfo/LoginServerInfo/WorldManager` config or DB, not hardcoded, unless this is strictly local test mode.

### 8. Packet framing and packet shapes must be treated as provisional

`GPacket` and `PacketBuffer` assume a simple frame/body model. Some packet names and comments are capture-confirmed, but many are provisional. Existing client compatibility requires packet shapes to be tagged by proof level:

```text
CONFIRMED_CAPTURE
CONFIRMED_PDB_NAME
PROVISIONAL_BODY
INVENTED_BODY
UNKNOWN
```

## Runtime robustness issues

### `PacketBuffer::Reserve` can fail silently

`PacketBuffer::Reserve` returns void and does this:

```cpp
uint8* p = (uint8*)realloc(...);
if (!p) return;
```

Write methods continue as if allocation succeeded, which can write through null or stale storage. Make `Reserve` return `bool`; every writer should fail cleanly or fatal-log before writing.

### IOCP send path is still blocking/pass-1

`Socket_Acceptor.cpp` uses blocking `send()` in `FlushSend_NoLock`, and `m_bSendInFlight` appears not to become a real overlapped send gate. This can pass smoke tests but is not a real original-style IOCP send pipeline yet.

### Session lifetime needs proof

`IOCPManager::WorkerLoop` calls `OnDisconnect()` and `Close()`, but ownership/delete/lifetime of accepted session objects was not proven in the static pass. Verify there is no leak and no use-after-close with outstanding overlapped operations.

## Protected quest SHN rule

The rewrite contains a guard in `Server/DataReader/ShnDataFileCheckSum.cpp`, but `ShnRegistry::LoadAll` appears to enumerate every `*.shn` under `Shine`, `Shine-1`, and `Shine/View` without applying the protected quest/PineScript guard.

Given the project rule, any tool/dev loader that parses SHNs should skip files where lowercase path/name contains:

```text
quest
pinescript
```

Runtime original server loading may need quest data eventually, but the provided parser/tool path should not touch those files unless a safe, proven reader is explicitly built.

## Stub/facade coverage

`HONEST_DISCLOSURE.md` admits 27 dead anonymous-namespace stub translation units. My static scan found many more tiny files; some are harmless façade files pointing to aggregate headers, but many are still function-surface placeholders.

Examples include:

```text
AuctionSystem.cpp
Chat.cpp
GuildTournamentSystem.cpp
ItemMall.cpp
MobBrain.cpp
StreetBoothBuy.cpp
TradeMisc.cpp
PartyBreak.cpp
```

Recommendation: classify each tiny file as one of:

```text
FACADE_TO_AGGREGATE_IMPL
DEAD_PDB_SLOT_STUB
COMPILE_PLACEHOLDER
REAL_MINIMAL_IMPLEMENTATION
```

Do not remove original-style names yet. The names are useful. Just make the status honest so Emergent does not confuse function-surface coverage for implemented behavior.

## What I would tell Emergent to do next

1. Fix the hard compile/link blockers above before adding any new systems.
2. Add or correct opcode aliases only when backed by PDB/capture/protocol evidence.
3. Add `RegisterZoneLuaAPI` properly and route Zone/AIScript/LuaScript through the same binding registration path.
4. Replace player-only target lookup with a unified `ShineObject` registry.
5. Move cooldown/skill state into the real per-character skill object.
6. Reshape combat into the original Roe function surface. Exact math can remain `VERIFY/TUNE` inside those functions.
7. Expand Lua bindings in priority order: damage, static damage, abstate, NPC dialog, rewards, timers, object lookup, map link, class change, KQ/instance helpers.
8. Keep all unknown values local inside the function body. Do not create new tuning docs/configs/tables for unknown math.
9. Keep `HONEST_DISCLOSURE.md` and expand it as features move from placeholder to real implementation.
10. Do not start client recode work until the server can pass login/world/zone/movement/NPC/item/combat smoke tests with the existing client.

## Included CSV audit files

- `undefined_nc_code_refs.csv` — undefined `NC_*` symbols referenced from code.
- `focused_missing_or_external_quoted_includes.csv` — focused include problems under Server/Client/Lua paths.
- `tiny_cpp_files_le12_code_lines.csv` — tiny `.cpp` files that need classification.
- `static_marker_hits.csv` — TODO/VERIFY/provisional/stub/no-op marker hits.
- `project_refs_summary.csv` — parsed `.vcxproj` reference counts.
- `project_missing_refs.csv` — empty in this pass.
- `source_files_not_in_vcxproj.csv` — empty in this pass for key source paths.
