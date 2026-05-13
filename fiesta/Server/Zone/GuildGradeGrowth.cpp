// Server/Zone/GuildGradeGrowth.cpp
// Guild grade/level progression -- 5 grades from GuildGradeData.shn.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class GuildGradeGrowth {
public:
    static GuildGradeGrowth& Get() { static GuildGradeGrowth s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("GuildGradeData") != NULL; }
};
} // namespace shine
