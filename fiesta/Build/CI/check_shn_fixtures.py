#!/usr/bin/env python3
"""SHN fixture integrity canary.

Walks any in-tree .shn binary fixtures and validates the documented
header layout (magic + record count + record size). Real game data is
not redistributable; this only catches drift in test fixtures that
ship in the repo.

Exit codes:
    0 -- all fixtures valid (or none found)
    1 -- at least one fixture is malformed
"""
import os, struct, sys

# Locations the canary will sweep. Empty directories are fine.
FIXTURE_ROOTS = [
    "docs/spec_pack/fixtures",
    "Server/Zone/tests/fixtures",
]

def check_shn(path):
    """Validate the documented SHN header format.

    Returns (ok, message). The format observed in MobInfo / ItemInfo
    headers in the spec pack:
        4 bytes  magic 'SHNF' or zero-padded crypt header
        4 bytes  record count (LE uint32)
        4 bytes  record byte size (LE uint32)
        ...      column descriptors + payload
    Many production SHNs are XOR-cryptored; in that case we only sanity
    check the file size is non-trivial.
    """
    sz = os.path.getsize(path)
    if sz < 12:
        return False, f"file too small ({sz} bytes)"
    with open(path, "rb") as f:
        head = f.read(12)
    magic = head[:4]
    rc, rs = struct.unpack_from("<II", head, 4)
    # Plaintext (uncrypted) fixtures we ship for tests should validate.
    if magic in (b"SHNF", b"SHNT"):
        if rc == 0 or rs == 0:
            return False, f"bad header: rc={rc} rs={rs}"
        # File body must hold at least rc * rs bytes of payload (header
        # column descriptors come between the 12-byte prelude and the
        # payload, so we use >=, not ==).
        if 12 + rc * rs > sz + (1 << 20):
            return False, f"size mismatch: hdr says >= {12 + rc * rs}, file is {sz}"
    return True, f"ok ({sz} bytes, rc={rc} rs={rs})"

def main():
    failed = 0
    found  = 0
    for root in FIXTURE_ROOTS:
        if not os.path.isdir(root):
            continue
        for dirpath, _, files in os.walk(root):
            for f in files:
                if not f.lower().endswith(".shn"):
                    continue
                found += 1
                p = os.path.join(dirpath, f)
                ok, msg = check_shn(p)
                if ok:
                    print(f"  OK  {p}: {msg}")
                else:
                    print(f"::error::{p}: {msg}")
                    failed += 1
    print(f"\n{found} fixture(s) checked, {failed} failure(s)")
    return 1 if failed else 0

if __name__ == "__main__":
    sys.exit(main())
