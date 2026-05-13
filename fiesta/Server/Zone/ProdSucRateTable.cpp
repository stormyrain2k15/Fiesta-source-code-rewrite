// Server/Zone/ProdSucRateTable.cpp
// Crafting success-rate table -- per-(level diff) and per-craft-class.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class ProdSucRateTable {
public:
    static ProdSucRateTable& Get() { static ProdSucRateTable s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("ProdSucRate") != NULL; }
    int32 RateX1k(int32 /*nLvDiff*/, uint8 /*uiClass*/) const { return 800; }   // 80% baseline
};
} // namespace shine
