"""
Walk every Shine-1/*.shn, parse with shn_parser.parse_shn(), emit a single
manifest CSV with: file, col_index, col_name, type, len.
Quest-style SHNs that fail are listed at the end.
"""
import os, sys, csv, traceback
sys.path.insert(0, os.path.dirname(__file__))
from shn_parser import parse_shn

SRC = '/app/downloads/Shine-1'
OUT = '/app/downloads/shn_schema_manifest.csv'
ROW_OUT = '/app/downloads/shn_row_counts.csv'
FAIL = '/app/downloads/shn_quest_or_failed.txt'

ok, failed = [], []
all_cols = []
row_counts = []

for fname in sorted(os.listdir(SRC)):
    if not fname.lower().endswith('.shn'):
        continue
    p = os.path.join(SRC, fname)
    try:
        cols, rows = parse_shn(p)
        ok.append(fname)
        row_counts.append((fname, len(rows), len(cols)))
        for i, c in enumerate(cols):
            all_cols.append((fname, i, c['name'], c['type'], c['len']))
    except Exception as e:
        failed.append((fname, str(e).splitlines()[0][:160]))

with open(OUT, 'w', newline='') as f:
    w = csv.writer(f)
    w.writerow(['file', 'col_index', 'col_name', 'type', 'len'])
    w.writerows(all_cols)

with open(ROW_OUT, 'w', newline='') as f:
    w = csv.writer(f)
    w.writerow(['file', 'rows', 'cols'])
    w.writerows(row_counts)

with open(FAIL, 'w') as f:
    for fn, err in failed:
        f.write(f"{fn}\t{err}\n")

print(f"OK: {len(ok)} files, {len(all_cols)} columns total")
print(f"FAILED: {len(failed)} files (likely quest SHNs)")
print(f"Manifest -> {OUT}")
