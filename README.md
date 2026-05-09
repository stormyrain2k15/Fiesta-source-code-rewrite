# Fiesta Online — Original-Style Engine Rewrite

Native C++ reimplementation of the NA2016-era Fiesta Online server and
client engine. The 43 gameplay systems all carry their original
class / function names so the layout matches the published PDB symbol
manifests one-for-one.

The actual engine source lives in [`fiesta/`](fiesta/). The full README
with build instructions, tuning surface, and audit notes is at
[`fiesta/README.md`](fiesta/README.md). What follows is the part you
should read **before** you check anything out.

---

## Project status — read this first

This is **not** a "drop into Visual Studio, hit F5, log in" build.

It is a **finish the code, tune some values, compile-debug,
runtime-debug** project. When that's done you have effectively the
same artifact Gamigo has — a fully-functional native source tree you
can shape any way you want. So the only real question left for the
project owner is:

> **How much do you want to make it your own?**

### Five things you should know up-front

1. **No client-manipulation code is present.** None of the original
   anti-cheat / client-tamper machinery has been carried over. If you
   want it back, design and add it cleanly.
2. **Client version check is removed.** Connecting clients are not
   gated on a build-stamp / version handshake.
3. **Zone authorization & "X trap" are removed.** The zone-side
   authority gate the original used to fence scripted clients is
   gone. Any equivalent enforcement is your call.
4. **All combat / skill / stat values are placeholders.** Everything
   compiles and *runs the right function*, but the constants
   (damage curves, hit / crit clamps, KQ rewards, AbState DoT
   scalers, Roe attack swing, gold / exp scalers, item-upgrade
   success rates, etc.) are first-pass numbers chosen to be sane,
   not balanced. Tuning is centralized — see
   [`fiesta/README.md`](fiesta/README.md) *Where to tune*.
5. **The next phase is your hands.** Compile-debug (VS2010, `v100`,
   `/MT`, MBCS, Win32) → runtime-debug → packet-capture parity →
   live-test. You do not need anyone else's source to finish this.

---

## What this is, and is not

- This is **fresh, originally-authored C++**. The class and function
  surface mirrors the original PDB symbol manifests; bodies are
  written from the published documentation and from the data layout
  in the SHN / TS / .dat files. Nothing in here is decompiled or
  restored from proprietary binaries.
- `fiesta/ThirdParty/Gamebryo/` is the vendored Gamebryo 2.3
  CoreLibs tree provided by the project owner. It is treated as a
  **read-only** third-party SDK.
- Code is strict **VS2010 / `v100`** (no C++11+ syntax).

## AbState is the central effect engine

All temporary effects — **player skills, monster skills, buffs,
debuffs, DoT / HoT, premium-item bonuses, special item effects,
map effects, mount effects, stat modifiers, movement / attack speed
changes, resistances, stuns / slows / disables, passive conditions,
temporary event effects** — route through `fiesta/Server/Zone/AbState`
and the `AbStateRuntime` dispatcher. If you add a system that
produces a temporary effect, route it through this pipeline; do not
invent a parallel ledger.

## Where to read next

- [`fiesta/README.md`](fiesta/README.md) — engine README, tuning
  surface, source-tree map.
- [`fiesta/docs/HONEST_DISCLOSURE.md`](fiesta/docs/HONEST_DISCLOSURE.md)
  — running audit log of every stub built out, every inferred column
  meaning, every PROVISIONAL packet shape waiting on a real capture.
- [`fiesta/docs/BUILD.md`](fiesta/docs/BUILD.md) — VS2010 build
  layout and the four DB exes.
- [`memory/PRD.md`](memory/PRD.md) — per-event SQL stored-proc
  routing table and pass-by-pass change log.
