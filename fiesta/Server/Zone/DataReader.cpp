// Server/Zone/DataReader.cpp
// Canonical-named alias for the DataReader registry. Existing
// /app/shine/Server/DataReader/ holds the actual loader; this file is the
// per-zone façade that fires the SHN ingest at boot.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class DataReader {
public:
    static bool LoadAll(const std::string& rRoot) {
        return ShnRegistry::Get().LoadAll(rRoot) > 0;
    }
};
} // namespace shine
