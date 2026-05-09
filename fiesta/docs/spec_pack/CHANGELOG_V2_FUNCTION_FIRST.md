# V2 Change Log — Function-First Rewrite Policy

Updated after Josh clarified that the missing parts are mostly math/functions whose values can be edited later.

Changes:

- Runtime captures are now optional validation, not a blocker for implementing systems.
- Unknown formula values must stay inside the owning function body as local tunables.
- the implementer must not create extra tuning docs/configs/SQL tables just because math is uncertain.
- Acceptance criteria now prioritize complete original-style function coverage.
- Added `08_FUNCTION_LOCAL_MATH_AND_NO_EXTRA_TUNING_DOCS.md`.
- Updated README, master requirements, system catalog, build order, capture workflow, and combined handoff.

Main rule: **the function has to be there first; exact values can be edited later in the function itself.**
