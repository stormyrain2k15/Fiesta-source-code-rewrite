#!/usr/bin/env python3
# Build/CI/audit_shn_wiring.py
#
# Cross-references the SHN file inventory in
#   docs/spec_pack/data_dictionary/File_Summary.csv
# against every "GetTable(...)" / "<stem>.shn" reference in the C++ tree
# and prints a coverage report.
#
# Pass criteria: every server-side SHN must be referenced by at least
# one consumer in /Server. Client-only view tables (the View/ subfolder)
# are listed separately and are NOT required to be wired -- the NA2016
# server intentionally does not consume them.
#
# Exit code: 0 = OK, 1 = a server-side SHN is unwired (regression).
#
# Run from anywhere; resolves paths relative to this file.

import csv
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", ".."))
SERVER = os.path.join(ROOT, "Server")
DICT = os.path.join(ROOT, "docs", "spec_pack", "data_dictionary",
                    "File_Summary.csv")

# Files in the View/ subfolder are client-only by design. Listed
# explicitly here -- if the server ever needs to own one, remove it
# from this set and add a consumer.
CLIENT_ONLY_VIEWS = set([
    "ActiveSkillView", "CharacterTitleStateView", "CollectCardView",
    "EffectViewInfo", "GTIView", "ItemShopView", "ItemViewDummy",
    "ItemViewEquipTypeInfo", "ItemViewInfo", "MapViewInfo",
    "MobConditionView", "MobViewInfo", "MoverSlotView", "MoverView",
    "PassiveSkillView", "ProduceView", "PupView", "SetItemView",
])

def main():
    if not os.path.exists(DICT):
        print("audit_shn_wiring: dictionary missing at", DICT, file=sys.stderr)
        return 0  # not a regression -- dictionary is optional artifact

    shn_stems = set()
    with open(DICT) as f:
        for r in csv.DictReader(f):
            p = r["File"]
            if p.lower().endswith(".shn"):
                shn_stems.add(os.path.splitext(os.path.basename(p))[0])

    blob_parts = []
    # Files to exclude from the source blob: their string literals are
    # NOT real consumers, just self-referential lists that would create
    # false PASS results.
    EXCLUDE_FILES = set([
        "ShnRegistry.cpp",  # contains kKnown[] view-name list
        "ShnRegistry.h",
    ])
    for root, _, files in os.walk(SERVER):
        for fn in files:
            if fn in EXCLUDE_FILES:
                continue
            if fn.endswith((".cpp", ".h")):
                with open(os.path.join(root, fn), errors="replace") as fp:
                    blob_parts.append(fp.read())
    blob = "\n".join(blob_parts)

    unwired_server = []
    unwired_view = []
    for stem in sorted(shn_stems):
        # A stem is considered "wired" if it appears in any recognized
        # SHN-table access form. The inline T("X") helper is the most
        # common pattern in MiscTables/ExtendedTables/GroupTables; the
        # bare "X" literal also catches macro-forwarded names.
        wired_forms = (
            'GetTable("%s"' % stem,        # ShnRegistry::Get().GetTable("X")
            'T("%s"' % stem,               # local T(name) inline accessor
            'BIND_BEGIN(t, "%s"' % stem,   # GroupTables binding macro
            '"%s"' % stem,                 # any other bare string-literal use
            "%s.shn" % stem,               # direct LoadFromFile path
        )
        if any(p in blob for p in wired_forms):
            continue
        if stem in CLIENT_ONLY_VIEWS:
            unwired_view.append(stem)
        else:
            unwired_server.append(stem)

    total = len(shn_stems)
    print("SHN wiring audit")
    print("=" * 50)
    print("Total SHN files in inventory: %d" % total)
    print("Client-only view tables:      %d (intentionally unwired)"
          % len(unwired_view))
    print("Server-wired:                 %d"
          % (total - len(unwired_server) - len(unwired_view)))
    print("Server-side gaps (REGRESSION): %d" % len(unwired_server))
    print()
    if unwired_server:
        print("UNWIRED SERVER TABLES:")
        for s in unwired_server:
            print("  -", s)
        print()
        print("FAIL: at least one server-side SHN has no consumer in /Server.")
        return 1

    print("PASS: 100% server-side SHN coverage.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
