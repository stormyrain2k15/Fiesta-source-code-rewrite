// Server/Zone/BattleHelpData.cpp
// Combat helper tables: hit-rate-vs-level curves, damage modifier curves,
// crit damage caps. Loaded from BattleHelp.shn.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class BattleHelpData {
public:
    static BattleHelpData& Get() { static BattleHelpData s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("BattleHelp") != NULL; }
};
} // namespace fiesta
