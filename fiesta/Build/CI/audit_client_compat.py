#!/usr/bin/env python3
"""Build/CI/audit_client_compat.py

Validates that every file under fiesta/Client/ stays compatible with the
project's hard VS2010 / MBCS / Win32 ceiling. The user's toolchain has no
C++11 STL (no <atomic>, <functional>, <thread>, <mutex>, etc.) and the
compiler rejects lambdas, range-for, nullptr, auto, and the inline
override / final / noexcept keywords are at best Microsoft extensions.

Exit code: 0 = clean, 1 = at least one violation.
"""
import os, re, sys

ROOT   = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
CLIENT = os.path.join(ROOT, "Client")

# Each rule: (regex, description). Hits in any client .cpp/.h fail the audit.
RULES = [
    # std headers introduced in C++11 (some have VS2010 workalikes; the
    # user explicitly forbids them so the codebase stays portable).
    (re.compile(r'^\s*#\s*include\s*<atomic>',          re.M), "<atomic> not in VS2010 std lib"),
    (re.compile(r'^\s*#\s*include\s*<functional>',      re.M), "<functional> -> use C function pointers"),
    (re.compile(r'^\s*#\s*include\s*<thread>',          re.M), "<thread> -> use CreateThread / _beginthreadex"),
    (re.compile(r'^\s*#\s*include\s*<mutex>',           re.M), "<mutex> -> use CRITICAL_SECTION"),
    (re.compile(r'^\s*#\s*include\s*<chrono>',          re.M), "<chrono> -> use GetTickCount"),
    (re.compile(r'^\s*#\s*include\s*<initializer_list>',re.M), "<initializer_list> not supported"),
    (re.compile(r'^\s*#\s*include\s*<unordered_map>',   re.M), "<unordered_map> -> use std::map"),
    (re.compile(r'^\s*#\s*include\s*<unordered_set>',   re.M), "<unordered_set> -> use std::set"),
    (re.compile(r'^\s*#\s*include\s*<array>',           re.M), "<array> -> use C array"),

    # C++11 std types/calls -- catch even without the include (chained from headers).
    (re.compile(r'\bstd::(atomic|function|thread|shared_ptr|unique_ptr|make_shared|make_unique|move|forward)\b'),
        "C++11 std:: type/function -- not allowed"),
    (re.compile(r'\bstd::(to_string|stoi|stoul|stof|stod)\b'),
        "C++11 string conversion helpers -- use snprintf / atoi / strtol"),

    # nullptr keyword.
    (re.compile(r'\bnullptr\b'),                       "nullptr -- use NULL"),

    # auto keyword (only as a type spec at the start of a decl). VS2010
    # supports the *old* auto storage-class so we have to be careful: only
    # `auto <ident>` with no leading type is forbidden. The regex below
    # matches that narrow case.
    (re.compile(r'(?:^|[\s;{(])auto\s+[a-zA-Z_]\w*\s*(?:[=:({]|\[)'),
        "auto type-deduction -- spell the type out"),

    # range-based for.
    (re.compile(r'for\s*\(\s*(?:const\s+)?[A-Za-z_:&*<> ]+\s+[a-zA-Z_]\w*\s*:\s*'),
        "range-based for -- use index iterator"),

    # lambda introducers `[capture](`.
    (re.compile(r'\[(?:this|=|&|[^\]\n]*?)\]\s*\([^)]*\)\s*(?:->|\{)'),
        "lambda expression -- use function pointer + void* ctx"),

    # `enum class` / scoped enum.
    (re.compile(r'\benum\s+(?:class|struct)\b'),
        "scoped enum -- use plain enum"),

    # `= delete` / `= default` member functions.
    (re.compile(r'=\s*(?:delete|default)\s*;'),
        "= delete / = default -- write the body yourself"),

    # constexpr / noexcept / final.
    (re.compile(r'\b(?:constexpr|noexcept|final)\b'),
        "constexpr / noexcept / final -- not supported"),

    # Trailing return type `auto foo(...) -> T`. We require `auto` somewhere
    # before the `)->` so we don't fire on expression-level `ptr->member`.
    (re.compile(r'\bauto\s+\w+\s*\([^)]*\)\s*->\s*[A-Za-z_]'),
        "trailing return type -- not supported"),
]

# `override` is a Microsoft extension since VS2008 and is accepted by
# VS2010 native code (see __cdecl override docs); we keep it.

# Files exempted from the audit. (Empty for now -- the patch is clean.)
EXEMPT = set()

def main():
    bad = []
    for r, _, fs in os.walk(CLIENT):
        for fn in fs:
            if not fn.endswith((".cpp", ".h", ".hpp")): continue
            p = os.path.join(r, fn)
            rel = os.path.relpath(p, ROOT).replace("\\", "/")
            if rel in EXEMPT: continue
            with open(p, encoding="utf-8", errors="replace") as fp:
                src = fp.read()
            for rx, desc in RULES:
                for m in rx.finditer(src):
                    # ignore comment lines
                    line_start = src.rfind("\n", 0, m.start()) + 1
                    line_end   = src.find("\n", m.end())
                    line = src[line_start:line_end].lstrip()
                    if line.startswith("//") or line.startswith("*") or line.startswith("/*"):
                        continue
                    lineno = src.count("\n", 0, m.start()) + 1
                    bad.append((rel, lineno, desc, line.strip()))

    if bad:
        print("REGRESSION: client VS2010-compat violations")
        print("="*72)
        for f, ln, desc, line in bad[:50]:
            print(f"  {f}:{ln}: {desc}")
            print(f"     {line[:96]}")
        if len(bad) > 50:
            print(f"  ... and {len(bad)-50} more")
        return 1
    print("OK: client tree is VS2010-clean.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
