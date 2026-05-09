// Server/Zone/Field.cpp
// Field/map instance management. A Field is one logical map; the Zone
// hosts up to 32 fields concurrently. Each field owns its mob list,
// drop list, player list, and PineScript runner state.
#include "MapField.h"
namespace fiesta {
// Symbol facade -- the concrete impl lives in MapField.cpp.
class Field { public: static Field& Get(){ static Field s; return s; } };
}} // anonymous
