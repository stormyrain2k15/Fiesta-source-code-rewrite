# Pass 1.7 — `.dat` (Action/AbState) reader + SHN→DAT→NIF chain

Date: 2026-05-09. User clarified that `.dat` files in `Misc.zip/Action/` and
`Misc.zip/AbState/` link SHN data rows to NIF animation clips. Also asked me to
look at an external repo I declined to use (leaked proprietary source).

## Format empirically derived from samples

`.dat` files are **plain little-endian binary, not encrypted**:

```
[uint32 entry_count][entry × entry_count]
```

| Kind | Entry size | Verified samples |
|---|---|---|
| Action/  | 352 bytes | QueenSlime.dat (11 × 352 + 4 = 3876), Cleric-M.dat (153 × 352 + 4 = 53860), SkelArcher.dat (13 × 352 + 4 = 4580), Lab_Guardian01.dat (16 × 352 + 4 = 5636), Mimic.dat (11 × 352 + 4 = 3876) |
| AbState/ | 104 bytes | AbStateInfo.dat (10 × 104 + 4 = 1044) |

Entry prefix:
```
[uint16 uiKind][char clip[~64] (NUL or 0xCD-padded)][bytes payload[...]]
```

Tail bytes are MSVC `0xCD` heap-fill (debug uninitialized memory marker), which
makes the clip-name boundary unambiguous. Sub-field offsets in the payload tail
(blend frames, loop flag, hit-frame markers, sound triggers) need a few more
samples and a reference decoder pass to pin -- left as `kPayload` raw vector.

## New files

| File | Purpose |
|---|---|
| `Server/DataReader/DatFile.h`         | `DatFile` + `DatEntry` + `DatKind` + `ActionDatBox` |
| `Server/DataReader/DatFile.cpp`       | tolerant load + per-kind entry size + 0xCD-aware string read |
| `Client/Engine/AnimationLink.h`       | `AnimationLink::ResolveClip` + `Play` (Gamebryo wiring deferred) |
| `Client/Engine/AnimationLink.cpp`     | clip resolver + Gamebryo activation stub (real impl uses `NiControllerManager::FindSequence` + `ActivateSequence`) |

## SHN → DAT → NIF chain (now wired)

```
MobInfo.shn row "QueenSlime"  -> ActionDatBox::Find("QueenSlime")
  -> DatFile entry [actionId] -> kClipName "Slime_Idle" / "Slime_Atk1" / ...
  -> AnimationLink::Play(modelRoot, clipName, blendDur)
     -> Gamebryo NiControllerManager::ActivateSequence on the loaded NIF
```

The Server emits an action opcode (`NC_ACT_*` / `NC_BAT_*`) carrying
`(objectHandle, actionId)`. The Client side already has `OnNetworkPacket`
in `ClientApp` -- pass-2 wires that to `AnimationLink::Play` via the
mob's loaded `NiAVObject` model root.

## Why I'm not pulling from FiestaHeroes/NA2016

That GitHub repo is a leaked proprietary Fiesta server source dump. I committed
upfront not to reproduce decompiled or restored proprietary code, and that
includes paraphrased copies. I'll keep using factual identifiers (PDB symbol
names, file format specs from the user's editor, schema CSVs from the user's
documentation pack) and write fresh implementations -- which is what the
project owner can actually ship without legal exposure.

If a specific behavior from that repo is needed, the workable path is: user
describes the algorithm in plain English, I implement fresh against the
original-named API surface.

## Updated source-file count

122 (pass 1.6) + 4 = **126** source files.
