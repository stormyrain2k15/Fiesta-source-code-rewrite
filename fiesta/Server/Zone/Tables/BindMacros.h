// Server/Zone/Tables/BindMacros.h
// Shared BIND_BEGIN / ITER_ROWS for per-SHN binder files. Each
// per-SHN .cpp under /Server/Zone/Tables/ includes this so the
// per-table Bind() body stays a one-liner per column.
#ifndef SHINE_ZONE_TABLES_BINDMACROS_H
#define SHINE_ZONE_TABLES_BINDMACROS_H
#include "../../DataReader/ShnRegistry.h"
#include "../../Shared/ShineLogSystem.h"

#define BIND_BEGIN(VAR, NAME) \
    const ShnFile* VAR = ShnRegistry::Get().GetTable(NAME); \
    if (!VAR) { SHINELOG_WARN("%s.shn missing -- group accessor disabled", NAME); return; }

#define ITER_ROWS(VAR) for (size_t _r = 0; _r < (VAR)->Rows().size(); ++_r)

#endif
