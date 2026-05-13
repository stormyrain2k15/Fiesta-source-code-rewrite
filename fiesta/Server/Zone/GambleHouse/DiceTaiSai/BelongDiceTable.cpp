// Server/Zone/GambleHouse/DiceTaiSai/BelongDiceTable.cpp
// "Belong" dice table -- per-bet payout multiplier table. Loaded from
// BelongDice.shn at boot.
#include "../../../DataReader/ShnRegistry.h"
namespace shine {
class BelongDiceTable {
public:
    static BelongDiceTable& Get() { static BelongDiceTable s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("BelongDice") != NULL; }
};
} // namespace shine
