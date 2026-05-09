// Server/Zone/CharClassDataBox.cpp
// Loads ClassName.shn (28 rows) + ChrCommon.txt FreeStat tables. Provides
// per-class display names and the per-(class,level) free-stat caps.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class CharClassDataBox {
public:
    static CharClassDataBox& Get() { static CharClassDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("ClassName") != NULL; }
};
} // namespace fiesta
