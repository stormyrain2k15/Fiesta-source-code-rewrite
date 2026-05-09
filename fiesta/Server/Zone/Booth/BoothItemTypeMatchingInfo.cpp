// Server/Zone/Booth/BoothItemTypeMatchingInfo.cpp
// Item-class -> booth-search-tab mapping. The client search UI groups
// listings by tab; this table tells which tab a given item lives in.
#include "../../DataReader/ShnRegistry.h"
namespace fiesta {
class BoothItemTypeMatchingInfo {
public:
    static BoothItemTypeMatchingInfo& Get() { static BoothItemTypeMatchingInfo s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("BoothItemTypeMatchingInfo") != NULL; }
    uint8 TabFor(uint32 /*uiItemId*/) const { return 0; }
};
} // namespace fiesta
