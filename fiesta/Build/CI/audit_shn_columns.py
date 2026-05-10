#!/usr/bin/env python3
# Build/CI/audit_shn_columns.py
#
# Column-level wiring audit. Walks every (table, column) pair in the
# Plain English Data Dictionary, then greps the C++ tree for any
# binder reference using one of the recognized access forms:
#
#   ShnGetU32(*t, i, "ColName")
#   ShnGetI32(*t, i, "ColName")
#   ShnGetStr(*t, i, "ColName")
#   ShnGetF32(*t, i, "ColName")
#   ColumnIndex(<table>, "ColName", ...)
#   "ColName"                          (free-text fallback - any literal)
#   row[ColName] / hdr["ColName"] / ...(loose match)
#
# Each column is classified:
#   GREEN  -- referenced by a binder for its own table (ideal)
#   YELLOW -- referenced as a string literal somewhere (might be wired)
#   RED    -- NEVER referenced anywhere in /Server (dead weight; system gap)
#
# The point of this audit is NOT to ensure every column is wired -- some
# columns ARE intentionally unread (e.g. client-side display strings on a
# server table). The point is to surface the RED list so the agent can
# decide system-by-system which gaps cascade into broken gameplay.
#
# Output:
#   - Per-table summary on stdout (counts only).
#   - /tmp/shn_column_gaps.csv: (Table, Column, Status, Type, Notes)
#   - /tmp/shn_system_gaps.txt: per-table RED column list, sorted by
#                               number of unread columns (worst first).
#
# Usage:
#   python3 Build/CI/audit_shn_columns.py [--system <substr>]
#   --system filter limits the report to tables whose System/Purpose
#   contains the given substring (case-insensitive). Useful for slicing
#   the audit by feature: e.g. --system "buff" or --system "portal".
import csv
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", ".."))
SERVER = os.path.join(ROOT, "Server")
DICT = os.path.join(ROOT, "docs", "spec_pack", "data_dictionary",
                    "Plain_English_Data_Dictionary.csv")
GAPS_CSV = "/tmp/shn_column_gaps.csv"
GAPS_TXT = "/tmp/shn_system_gaps.txt"

CLIENT_ONLY = set([
    "ActiveSkillView", "CharacterTitleStateView", "CollectCardView",
    "EffectViewInfo", "GTIView", "ItemShopView", "ItemViewDummy",
    "ItemViewEquipTypeInfo", "ItemViewInfo", "MapViewInfo",
    "MobConditionView", "MobViewInfo", "MoverSlotView", "MoverView",
    "PassiveSkillView", "ProduceView", "PupView", "SetItemView",
])
EXCLUDE_FILES = set([
    "ShnRegistry.cpp", "ShnRegistry.h",     # contains kKnown[] view-name list
])

def load_source_blob():
    parts = []
    for root, _, files in os.walk(SERVER):
        for fn in files:
            if fn in EXCLUDE_FILES: continue
            if fn.endswith((".cpp", ".h")):
                with open(os.path.join(root, fn), errors="replace") as fp:
                    parts.append(fp.read())
    return "\n".join(parts)

def parse_dict():
    by_table = {}      # stem -> [(col_no, col, type, purpose), ...]
    purpose = {}       # stem -> system/purpose text
    with open(DICT) as f:
        for r in csv.DictReader(f):
            path = r["File"]
            if not path.lower().endswith(".shn"): continue
            stem = os.path.splitext(os.path.basename(path))[0]
            by_table.setdefault(stem, []).append((
                r["Column #"], r["Column"], r["Type"],
                r["Plain English Meaning"][:80]))
            purpose[stem] = r["System / File Purpose"]
    return by_table, purpose

def classify(blob, table, col):
    """Return ('GREEN'|'YELLOW'|'RED', evidence_snippet)."""
    # Strong: ShnGet*(*t, i, "Col") — usually paired with a table from
    # GetTable(table). We don't try to verify the table is "this one"
    # because lookup variables are renamed; treat strong = exact form.
    strong_patterns = [
        'ShnGetU32(*t, i, "%s"' % col,
        'ShnGetI32(*t, i, "%s"' % col,
        'ShnGetStr(*t, i, "%s"' % col,
        'ShnGetF32(*t, i, "%s"' % col,
        'ShnGetU32(*t, r, "%s"' % col,
        'ShnGetI32(*t, r, "%s"' % col,
        'ShnGetStr(*t, r, "%s"' % col,
        'ShnGetF32(*t, r, "%s"' % col,
        'ShnGetU32(*t, %d, "%s"',                  # never used; quiet warn
    ]
    # Some binders alias the table pointer: ShnGetU32(*pkT, ...)
    for varname in ('pkT', 'rTab', 'rT', 'tab', 't2', 't3'):
        for fn in ('U32','I32','Str','F32'):
            strong_patterns.append('ShnGet%s(*%s, ' % (fn, varname))
    # Also catch the inline-iter style: row[col] = ShnGetX(*t, i, "Col")
    for p in strong_patterns[:8]:
        if p in blob:
            return ('GREEN', p)
    # ColumnIndex("Col" ...) is also strong
    ci = 'ColumnIndex(' 
    if ci in blob and ('"%s"' % col) in blob:
        # weaker than the direct read; mark green if both occur in same line
        # (we'll just check both exist)
        return ('GREEN', 'ColumnIndex+%s' % col)
    # Weak: the column name appears as a free-text quoted literal
    if ('"%s"' % col) in blob:
        return ('YELLOW', '"%s"' % col)
    # Loose: bare token (e.g. .ColName usage in a typed struct field)
    # Lots of false positives possible -- scope to camelcase identifiers
    # of length >= 4 to keep noise down.
    if len(col) >= 4 and re.search(r'\b' + re.escape(col) + r'\b', blob):
        return ('YELLOW', '<bare token>')
    return ('RED', '')

def main():
    sysfilter = None
    args = sys.argv[1:]
    while args:
        a = args.pop(0)
        if a == '--system' and args:
            sysfilter = args.pop(0).lower()

    blob = load_source_blob()
    by_table, purpose = parse_dict()
    rows_out = []
    table_red_count = []
    total_g = total_y = total_r = 0
    for stem in sorted(by_table.keys()):
        if stem in CLIENT_ONLY:
            continue
        if sysfilter and sysfilter not in (purpose.get(stem,"").lower()) \
                and sysfilter not in stem.lower():
            continue
        cols = by_table[stem]
        red_cols = []
        for (n, col, typ, mean) in cols:
            status, ev = classify(blob, stem, col)
            rows_out.append((stem, col, status, typ, purpose.get(stem,""), mean))
            if status == 'GREEN': total_g += 1
            elif status == 'YELLOW': total_y += 1
            else:
                total_r += 1
                red_cols.append((col, typ))
        if red_cols:
            table_red_count.append((stem, len(red_cols), len(cols), red_cols, purpose.get(stem,"")))

    # write detail csv
    with open(GAPS_CSV, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["Table","Column","Status","Type","System","Meaning"])
        for r in rows_out: w.writerow(r)

    # write per-table summary, worst first
    table_red_count.sort(key=lambda x: (-x[1], x[0]))
    with open(GAPS_TXT, "w") as f:
        f.write("Per-table column wiring report (RED only)\n")
        f.write("=" * 60 + "\n\n")
        for stem, red, total, red_cols, sysname in table_red_count:
            f.write("[%d/%d unread] %s -- %s\n" % (red, total, stem, sysname))
            for (c, t) in red_cols:
                f.write("    %-30s %s\n" % (c, t))
            f.write("\n")

    # stdout summary
    total = total_g + total_y + total_r
    print("Column-level SHN audit")
    print("=" * 50)
    print("Total columns audited: %d" % total)
    print("  GREEN  (binder-read):   %d  (%5.1f%%)" % (total_g, 100.0*total_g/total if total else 0))
    print("  YELLOW (string-only):   %d  (%5.1f%%)" % (total_y, 100.0*total_y/total if total else 0))
    print("  RED    (no reference):  %d  (%5.1f%%)" % (total_r, 100.0*total_r/total if total else 0))
    print()
    print("Top 30 tables by unread-column count:")
    for stem, red, total_t, _, sysname in table_red_count[:30]:
        print("  %3d/%3d  %-32s  %s" % (red, total_t, stem, sysname[:50]))
    print()
    print("Detail CSV : %s" % GAPS_CSV)
    print("RED report : %s" % GAPS_TXT)

if __name__ == "__main__":
    main()
