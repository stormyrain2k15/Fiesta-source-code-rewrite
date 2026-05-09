// Server/Zone/BossFieldData.cpp
// Boss-field configuration -- per-map list of recurring boss spawns
// (boss id, respawn cadence, alert broadcast, drop overrides).
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class BossFieldData {
public:
    static BossFieldData& Get() { static BossFieldData s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("BossField") != NULL; }
};
} // namespace fiesta
