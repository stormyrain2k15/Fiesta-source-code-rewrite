#!/usr/bin/env python3
"""Doc cross-link canary.

Scans every .md under docs/ and the project root for relative links
of the form `[label](../path/to/file.md)` and resolves them on disk.
A broken link fails the canary so a refactor that moves a doc gets
caught immediately.
"""
import os, re, sys

LINK_RE = re.compile(r"\[[^\]]+\]\(([^)#?\s]+)\)")
ROOTS = ["docs", "."]

def is_local(target):
    if target.startswith(("http://", "https://", "mailto:")):
        return False
    return True

def main():
    failed = 0
    seen = set()
    for root in ROOTS:
        for dirpath, _, files in os.walk(root):
            # Skip the canary dirs themselves and ThirdParty.
            if "ThirdParty" in dirpath or ".git" in dirpath:
                continue
            for f in files:
                if not f.endswith(".md"): continue
                p = os.path.join(dirpath, f)
                if p in seen: continue
                seen.add(p)
                with open(p, "r", encoding="utf-8", errors="ignore") as fh:
                    text = fh.read()
                for m in LINK_RE.finditer(text):
                    target = m.group(1).strip()
                    if not is_local(target): continue
                    # Resolve relative to the doc that contains the link.
                    resolved = os.path.normpath(os.path.join(os.path.dirname(p), target))
                    if not os.path.exists(resolved):
                        print(f"::error file={p}::broken link to '{target}' (resolved: {resolved})")
                        failed += 1
    if failed == 0:
        print(f"All doc links resolve OK ({len(seen)} files scanned).")
    return 1 if failed else 0

if __name__ == "__main__":
    sys.exit(main())
