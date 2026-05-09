// Server/Zone/MapBlockInformation.h
#ifndef FIESTA_ZONE_MAPBLOCKINFORMATION_H
#define FIESTA_ZONE_MAPBLOCKINFORMATION_H
#include "../Shared/ShineTypes.h"
#include <string>

namespace fiesta {
class MapBlockInformation {
public:
    static MapBlockInformation& Get();
    bool Load(uint16 uiMap, const std::string& rPath);
    bool IsBlocked(uint16 uiMap, float x, float y) const;
};
} // namespace fiesta
#endif
