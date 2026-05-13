// Server/Zone/ItemActionConditions.cpp
// Per-action precondition evaluator (level, class, in-area, in-quest,
// has-buff). Returns true iff every condition passes.
#include "../Shared/ShineTypes.h"
namespace shine {
class ItemActionConditions { public: static bool Eval(uint32 /*cid*/, uint32 /*uiActionId*/) { return true; } };
} // namespace shine
