// Server/Zone/SpamerPenaltyDataBox.h
// WIRE-17 (Lyra, May 2026)
#ifndef FIESTA_ZONE_SPAMERPENALTYDATABOX_H
#define FIESTA_ZONE_SPAMERPENALTYDATABOX_H
#include "../Shared/ShineTypes.h"
namespace fiesta {
class SpamerPenaltyDataBox {
public:
    static SpamerPenaltyDataBox& Get();
    bool   Load();
    uint32 PenaltySecs(uint8 uiOffenseCount) const;
};
} // namespace fiesta
#endif
