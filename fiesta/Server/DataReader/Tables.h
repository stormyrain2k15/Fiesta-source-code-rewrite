// Server/DataReader/Tables.h  -- retired in favor of Schemas.h.
// All headline tables are now declared in Schemas.h with full field sets sourced
// from the documentation integration pack. This shim exists only so existing
// includes of "Tables.h" keep compiling; new code should include "Schemas.h".
#ifndef FIESTA_DATAREADER_TABLES_H
#define FIESTA_DATAREADER_TABLES_H
#include "Schemas.h"
namespace fiesta {
// Back-compat aliases for code that referenced the pass-1 example structs.
typedef ItemInfoRow         ItemInfoRec;
typedef AbStateRow          AbnormalStateInfoRec;
typedef ActiveSkillRow      ActiveSkillInfoRec;
typedef ChargedEffectRow    ChargedItemEffectRec;
} // namespace fiesta
#endif
