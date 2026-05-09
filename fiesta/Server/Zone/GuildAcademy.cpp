// Server/Zone/GuildAcademy.cpp
// Guild academy -- low-level member buff zone. Loaded from
// GuildAcademy.shn (StaAcaGenalBuf, 60s join time).
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class GuildAcademy {
public:
    static GuildAcademy& Get() { static GuildAcademy s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("GuildAcademy") != NULL; }
};
}} // anonymous
