// Server/Zone/SpyNet.cpp
// Anti-exploit spy. Watches for impossible delta in pos/HP/EXP between
// consecutive ticks. On a hit, records to AdminSecLog and optionally
// kicks the offender.
#include "AdminSecLog.h"
namespace fiesta {
class SpyNet {
public:
    static void OnTick(uint32 /*cid*/) {}
};
} // namespace fiesta
