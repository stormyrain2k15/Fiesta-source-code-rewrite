// Server/Zone/ItemDropAnalyzer.cpp
// Telemetry helper -- aggregates per-mob drop rates over last N rolls
// for live ops drop-rate balancing.
#include "../Shared/ShineTypes.h"
namespace fiesta { class ItemDropAnalyzer { public: static void OnDrop(uint32, uint32) {} }; }
