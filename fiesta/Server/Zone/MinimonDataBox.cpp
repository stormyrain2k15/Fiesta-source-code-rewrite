// Server/Zone/MinimonDataBox.cpp
// Mini-pet (minimon) data -- non-combat companion stats and skin.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class MinimonDataBox {
public:
    static MinimonDataBox& Get() { static MinimonDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("Minimon") != NULL; }
};
}} // anonymous
