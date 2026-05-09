# Capture-to-Code Workflow

## Current rewrite policy update

Runtime captures are useful validation, but they are not required before implementing the missing function surface. Emergent should implement every original-style system/function first using PDB names, docs, SHNs/TXTs, and source/module evidence. Captures/Hex-Rays are then used to tune and verify packet layouts, DB writes, and exact outputs.

Unknown gameplay math should remain inside the owning function body as local `VERIFY/TUNE` constants or terms. Do not add a separate balance document/config system just because a value is unknown.


Josh's weekend run should become source pages, not a giant packet soup.

## Evidence chain

Player action → FiestaShark wire packet → rat IOCP/internal packet → Olly live call stack → IDA/Hex-Rays handler → PDB symbol → DB write → server response → client UI/state change.

## Event folder standard

```txt
EVENT_042_job_change_attempt_01/
  notes.txt
  fiestashark_both_sides.txt
  rat_zone_iocp.log
  rat_wm_iocp.log
  olly_zone_callstack.log
  ida_hexrays_handler.c
  ida_xrefs.txt
  db_writes.txt
  db_before_after.txt
```

## Repetition rule

Repeat actions 3-5 times inside the same capture with idle gaps. This separates baseline noise from action-triggered packets.

## Noise rule

Do not delete high-frequency packets. Classify them as idle baseline, state tick, heartbeat, entity update, control packet, action trigger, action response, DB-backed state change, or unknown high priority.

## Dungeon/instance special run

Use immortal/static-damage capture to expose: instance create, MID role validation, map allocation, Lua/static damage path, objectives, boss spawn, reward, exit, teardown.

## Hex-Rays export rule

One file per handler/action chain. Export dispatch case, handler, close helpers, table lookups, response builder, SQL call path. Avoid one monster dump.

