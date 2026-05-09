// Server/Zone/MapBlockInformation.h
// Pure forward to MapField.h. The canonical definition of
// `MapBlockInformation` (the per-Field collision grid struct) lives in
// MapField.h; this header exists so legacy includes that say
// `#include "MapBlockInformation.h"` keep working without dragging in
// the rest of the world hierarchy.
#ifndef FIESTA_ZONE_MAPBLOCKINFORMATION_H
#define FIESTA_ZONE_MAPBLOCKINFORMATION_H
#include "MapField.h"
#endif
