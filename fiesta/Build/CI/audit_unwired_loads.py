#!/usr/bin/env python3
# Build/CI/audit_unwired_loads.py
#
# Finds every `bool X::Load(...)` / `void X::Bind()` defined in the
# server tree but never called from any boot path. A miss here means
# the table is parsed at boot but the binder that consumes it never
# runs, which silently breaks the feature it owns (the canonical
# example: LevelGapTable was defined but never called for ~12 months,
# silently disabling per-level PvP scaling).
#
# Exit code 0 = pass. Non-zero = at least one new orphan introduced.
import os, re, sys

ROOT = os.path.normpath(os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "..", ".."))
SERVER = os.path.join(ROOT, "Server")

# Files that count as "boot code" -- if a Load/Bind is referenced here,
# it's wired.
BOOT_FILES = [
    "Server/Zone/Main.cpp",
    "Server/Zone/ZoneAssetLoader.cpp",
    "Server/Zone/MoverTables.cpp",
    "Server/Zone/GroupTables.cpp",     # BindAllGroupTables
    "Server/Zone/MiscTables.cpp",      # BindAllMiscTables
    "Server/Zone/MoreTables.cpp",      # BindAllMoreTables
    "Server/Zone/ExtendedTables.cpp",  # BindAllExtendedTables
    "Server/Zone/TypedSchemaConsumers.cpp",
    "Server/Zone/ChargedEffect.cpp",
    "Server/Zone/GambleHouse/GambleSystem.cpp",  # BindAllCasinoTables
    "Server/WorldManager/Main.cpp",
    "Server/Login/Main.cpp",
    "Server/DataReader/DataBox.cpp",
]

# Classes whose Load/Bind is intentionally per-file or per-character,
# not boot-singleton. They're owned by ZoneAssetLoader's directory
# walker (per-file Load) or per-character handlers (per-cid Load),
# not by the Get().Load() singleton pattern.
INTENTIONAL_NON_BOOT = set([
    # Per-file walkers (called from inside ZoneAssetLoader's Walk*).
    "MobAttackSequence", "MobBehaviorScript", "MobRegenTable",
    "MobRoamMap", "MobSettingActionFile", "NPCItemListFile",
    "PsScriptFile", "ScenarioScript", "ScriptStringTable",
    "AIScript", "FieldTable",
    # Per-character (called on login).
    "AbnormalStateShelter",
    # Per-name Lua loaders (called from ZoneAssetLoader::WalkAllLua
    # in a name-driven loop, not from the Get().Load(rRoot) pattern).
    "InstanceSystem", "PromoteSystem",
    # Infrastructure (called via different entry, not Get().X()).
    "IOCPManager", "ServerInfo", "ConfigParser", "ShnRegistry",
    "DataBox", "ShnFile", "TableScriptFile", "ZoneAssetLoader",
    "QuestShnReader", "Schemas", "DataReader", "SQLP", "SQLP_Estate",
])

sig_re = re.compile(
    r'^(?:bool|void|size_t)\s+([A-Z][A-Za-z0-9_]+)::(Load|Bind|LoadAll|BindAll)\s*\(',
    re.M)

def is_called(boot_blob, cls, fn):
    pat = r'\b%s\s*::\s*Get\s*\(\s*\)\s*\.\s*%s\s*\(' \
          % (re.escape(cls), re.escape(fn))
    return bool(re.search(pat, boot_blob))

def main():
    defs = set()
    for r,_,fs in os.walk(SERVER):
        for fn in fs:
            if fn.endswith('.cpp'):
                p = os.path.join(r, fn)
                with open(p, errors='replace') as fp: src = fp.read()
                for m in sig_re.finditer(src):
                    defs.add((m.group(1), m.group(2), p))

    boot_blob = ""
    for f in BOOT_FILES:
        full = os.path.join(ROOT, f)
        if os.path.exists(full):
            with open(full, errors='replace') as fp:
                boot_blob += fp.read() + "\n"

    bad = []
    for cls, fn, p in sorted(defs):
        if cls in INTENTIONAL_NON_BOOT: continue
        if cls.endswith("Tab"):                # *Tab::Load -- via DataBox
            continue
        if is_called(boot_blob, cls, fn): continue
        bad.append((cls, fn, p.replace(ROOT + "/", "")))

    if bad:
        print("REGRESSION: orphaned data-load methods (no boot call)")
        print("="*60)
        for cls, fn, p in bad:
            print("  %s::%s    (%s)" % (cls, fn, p))
        print()
        print("Add a `%s::Get().%s(...)` call from a boot file, OR add"
              % (bad[0][0], bad[0][1]))
        print("the class to INTENTIONAL_NON_BOOT in this script if it")
        print("genuinely runs from a non-singleton path.")
        return 1

    print("OK: every Load/Bind has a boot path.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
