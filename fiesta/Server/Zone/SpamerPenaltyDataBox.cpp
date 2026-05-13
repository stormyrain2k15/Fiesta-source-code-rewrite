// Server/Zone/SpamerPenaltyDataBox.cpp
// #include "SpamerPenaltyDataBox.h"
#include "../DataReader/ShnRegistry.h"
namespace shine {
SpamerPenaltyDataBox& SpamerPenaltyDataBox::Get() { static SpamerPenaltyDataBox s; return s; }
bool SpamerPenaltyDataBox::Load() {
    return ShnRegistry::Get().GetTable("SpamerPenalty") != NULL;
}
uint32 SpamerPenaltyDataBox::PenaltySecs(uint8 uiOffenseCount) const {
    switch (uiOffenseCount) {
        case 1: return 5 * 60;
        case 2: return 30 * 60;
        case 3: return 24 * 3600;
        default: return 0xFFFFFFFFu; // permanent
    }
}
} // namespace shine
