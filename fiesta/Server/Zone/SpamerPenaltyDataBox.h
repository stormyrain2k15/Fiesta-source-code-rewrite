// Server/Zone/SpamerPenaltyDataBox.h
#ifndef SHINE_ZONE_SPAMERPENALTYDATABOX_H
#define SHINE_ZONE_SPAMERPENALTYDATABOX_H
#include "../Shared/ShineTypes.h"
namespace shine {
class SpamerPenaltyDataBox {
public:
    static SpamerPenaltyDataBox& Get();
    bool   Load();
    uint32 PenaltySecs(uint8 uiOffenseCount) const;
};
} // namespace shine
#endif
