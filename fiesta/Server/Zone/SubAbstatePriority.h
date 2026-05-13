// Server/Zone/SubAbstatePriority.h
// PDB-name anchor for `SubAbstatePriority`. The class is declared
// alongside `AbnormalState` in `AbState.h` (they are tightly coupled --
// the priority resolver reads `AbStateRow` columns, and `AbnormalState`
// applies its decisions when stacking new instances). This header is
// kept as a forwarder so legacy includes still resolve.
#ifndef SHINE_ZONE_SUBABSTATEPRIORITY_H
#define SHINE_ZONE_SUBABSTATEPRIORITY_H
#include "AbState.h"   // exposes class SubAbstatePriority
#endif
