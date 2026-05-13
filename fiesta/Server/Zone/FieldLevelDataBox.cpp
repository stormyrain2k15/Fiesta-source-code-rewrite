// Server/Zone/FieldLevelDataBox.cpp
// Per-(map, level-tier) tuning -- exp scalar, drop scalar, fame scalar.
// Sourced from FieldLvCondition.shn.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class FieldLevelDataBox {
public:
    static FieldLevelDataBox& Get() { static FieldLevelDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("FieldLvCondition") != NULL; }
};
} // namespace shine
