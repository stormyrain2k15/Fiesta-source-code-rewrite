// Server/Zone/MultiHitTable.cpp
// MultiHit data box. Class declared in SkillSystem.h; this file owns
// the canonical filename and the table-load shim.
#include "../DataReader/ShnRegistry.h"
#include "SkillSystem.h"
namespace fiesta { namespace {
inline bool MultiHitTable_Load() { return ShnRegistry::Get().GetTable("MultiHit") != NULL; }
}} // anonymous
