# Function-Local Math Policy — No Invented Tuning Documents

This rewrite should look like original Fiesta server source, not a modern balance framework layered on top of it.

## Core rule

If a system/function exists in the original game, the implementer must implement the function even when the exact formula constants are not known yet.

Unknown math is handled inside the function body:

- use the original-style function/module/class name from PDBs when available;
- place the provisional formula directly in the owning function;
- keep uncertain constants as local `const` values or local variables;
- mark them with `// VERIFY/TUNE:` comments;
- tune them later by editing that function, not by creating new external tuning files.

## Do not invent new tuning layers

Do not create any of these unless original evidence shows the real server had them:

- new balance markdown docs;
- new JSON/YAML/INI tuning files;
- new SQL balance tables;
- new generated spreadsheets;
- new global formula registry;
- new script-only formula override layer.

Original Fiesta already has many data sources: SHN, TXT, Lua, PineScript, SQL, service config, and hardcoded C++ functions. Use those original-style locations only.

## What belongs in code

These can be provisional inside the owning function:

- Roe damage constants;
- hit/miss/crit/block curves;
- stat compile coefficients;
- level-gap modifiers;
- item upgrade chance math;
- random option roll weights;
- set-effect aggregation terms;
- premium/charm/extender modifiers;
- mob AI weights;
- dungeon reward/static-damage values;
- class/job stat recalculation terms.

## Example

```cpp
int normalpyRoe_damage(/* original-ish args */)
{
    // VERIFY/TUNE: provisional physical damage curve. Do not move to external config unless original evidence is found.
    const int kMinimumDamage = 1;
    const int nAttack = Roe_getattack(/*...*/);
    const int nDefense = Roe_defendpower(/*...*/);

    int nDamage = (nAttack * nAttack) / max(1, nAttack + nDefense);
    if (nDamage < kMinimumDamage)
        nDamage = kMinimumDamage;

    return nDamage;
}
```

## Acceptance rule

A system is acceptable for Pass 2 if:

1. the function/class/module exists with original-style naming;
2. the function is in the correct subsystem;
3. it uses original SHN/TXT/SQL/Lua data when known;
4. any unknown math is implemented locally with `VERIFY/TUNE` comments;
5. no invented external tuning document/config layer was created.

Exact numeric output can be tuned later. Missing function coverage cannot.
