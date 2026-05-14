# Shine Engine — Original-Style Engine Rewrite

Native C++ MMO server and client engine, originally bootstrapped from
the NA2016-era documentation pack and now evolving into a standalone
product. The 43 gameplay systems all carry their original
class / function names so the layout matches the published PDB symbol
manifests one-for-one.

---

## Project status — read this first

This is **not** a "drop into Visual Studio, hit F5, log in" build.

It is a **finish the code, tune some values, compile-debug, runtime-debug**
project. When that's done you have effectively the same artifact Gamigo
has — a fully-functional native source tree you can shape any way you
want. So the only real question left for the project owner is:

> **How much do you want to make it your own?**

### Five things you should know up-front

1. **No client-manipulation code is present.** None of the original
   anti-cheat / client-tamper machinery has been carried over. If you
   want it back, design and add it cleanly.
2. **Client version check is removed.** Connecting clients are not
   gated on a build-stamp/version handshake.
3. **Zone authorization & "X trap" are removed.** The zone-side
   authority gate the original used to fence scripted clients is gone.
   Any equivalent enforcement is your call.
4. **All combat / skill / stat values are placeholders.** Everything
   compiles and *runs the right function*, but the constants
   (damage curves, hit/crit clamps, KQ rewards, AbState DoT scalers,
   Roe attack swing, gold/exp scalers, item-upgrade success rates,
   etc.) are first-pass numbers chosen to be sane, not balanced.
   Tuning is centralized — see *Where to tune* below.
5. **The next phase is your hands.** Compile-debug (VS2010, `v100`,
   `/MT`, MBCS, Win32) → runtime-debug → packet-capture parity →
   live-test. You do not need anyone else's source to finish this.

---

## What this is, and is not

- This is **fresh, originally-authored C++**. The class and function
  surface mirrors the original PDB symbol manifests; bodies are written
  from the published documentation and from the data layout in the SHN /
  TS / .dat files. Nothing in here is decompiled or restored from
  proprietary binaries.
- `ThirdParty/Gamebryo/` is the vendored Gamebryo 2.3 CoreLibs tree
  provided by the project owner. It is treated as a **read-only**
  third-party SDK: the client `#include`s its headers and links the
  libraries you build. Do not modify files under `ThirdParty/Gamebryo/`.
- Code is strict **VS2010 / `v100`** (no C++11+ syntax, no range-based
  `for`, no `auto`, no lambdas).

## Where to tune

Every "function known, values unknown" knob lives in a single header
or near the call site so a re-balancing pass is a one-file edit:

| File | Owns |
|------|------|
| `Server/Zone/BattleTunables.h` | Combat curves, Roe ATK/DEF/Hit/Crit, AbState action constants + DoT scaler + tick cadence, KQ reward base/perPoint/cap, well-known GMEvent ids |
| `Server/Zone/KingdomQuest.cpp` | KQ state-machine timings (Recruit / Running / Vote / End / MinPlayers) |
| `Server/Zone/CharDBClient.cpp` | AbState persistence stored-proc ids (90 / 91) |
| `Server/Zone/ItemUpgrade.cpp` | Upgrade success / luck-stone curve scaler |

`docs/HONEST_DISCLOSURE.md` is the running audit log of every stub
that's been built out, every column whose meaning was inferred, and
every PROVISIONAL packet shape waiting on a real capture.

## Build and run

VS2010 toolset (`v100`), Win32 only. See `docs/BUILD.md` for the project
file layout and the four DB exes (`Account`, `AccountLog`, `Character`,
`GameLog`).

## Source-tree map

```
Server/Shared      foundation: timers, packet, crypto, IOCP, log
Server/Common      NETCOMMAND, ProtocolParser, GPacket
Server/DataReader  SHN / TS / DAT / BMP loaders + ITableBase
Server/DataServer  the four DB exes
Server/Login       credential authority
Server/WorldManager  world router + OPTool loopback
Server/Zone        gameplay engine + 43 subsystems
Lua/               Lua 5.2 host + cBindings
Client/            client shell + Gamebryo bootstrap
ThirdParty/Gamebryo/  read-only vendored renderer
docs/              build notes + spec pack
```

## AbState is the central effect engine

All temporary effects — **player skills, monster skills, buffs,
debuffs, DoT/HoT, premium-item bonuses, special item effects,
map effects, mount effects, stat modifiers, movement / attack
speed changes, resistances, stuns / slows / disables, passive
conditions, temporary event effects** — route through
`Server/Zone/AbState.{h,cpp}` and the `AbStateRuntime` dispatcher.
If you add a system that produces a temporary effect, route it
through this pipeline; do not invent a parallel ledger.

The buff schema (`AbState.shn` 777 rows + `SubAbState.shn` 2041 rows)
is decoded into typed records. Each runtime row carries up to four
`(ActionIndex, ActionArg)` slots dispatched by `AbStateRuntime::Tick`,
plus shield (`AbsorbIncoming`) and reflect (`ReflectIncoming`) gates
that `Battle::Apply` consults before / after HP loss.

## Persistent state

Every gameplay event that mutates lasting character state issues a real
SQL stored-proc call through one of the DB exes. See `memory/PRD.md` for
the per-event routing table.

## Data ingest

`9Data/Shine/<Map>/...` is walked at boot by `ZoneAssetLoader`. Every
SHN column is either hand-bound by a typed reader or audited by
`ShnAudit_EmitReport`; every World/*.txt column the same way via
`TsAudit_EmitReport`. Unread columns surface as a single WARN line per
`(table.column)` in the boot log.

The canonical schema reference lives in
`docs/spec_pack/data_dictionary/` (File_Summary, plain-English column
dictionary, server-link hints). `docs/SCHEMA_COVERAGE.md` is the
human-readable wiring report; `Build/CI/audit_shn_wiring.py` enforces
100% server-side coverage on every push (fails CI on regression).
