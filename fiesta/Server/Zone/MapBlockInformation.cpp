// Server/Zone/MapBlockInformation.cpp
// MapBlockInformation is a struct owned by each Field (defined in
// MapField.h). The cell payload is loaded from BlockInfo/<MapName>.shbd
// at boot by ZoneAssetLoader::WalkBlockInfo + LoadShbd. This TU is kept
// so the project file slot is preserved; no extra runtime symbols are
// emitted because all behaviour lives inline in MapField.h.
#include "MapBlockInformation.h"
namespace shine { /* see MapField.h */ }
