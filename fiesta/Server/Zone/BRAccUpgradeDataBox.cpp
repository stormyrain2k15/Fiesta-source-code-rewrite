// Server/Zone/BRAccUpgradeDataBox.cpp
// Brazilian-region accessory upgrade overrides. Same shape, different
// numbers (regional balance pass).
#include "../DataReader/ShnRegistry.h"
namespace shine { namespace {
class BRAccUpgradeDataBox {
public:
    static BRAccUpgradeDataBox& Get() { static BRAccUpgradeDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("BRAccUpgrade") != NULL; }
};
}} // anonymous
