// Server/Zone/MapClusterManager.cpp
// Map cluster -- a set of maps that share a single field instance. The
// town + nearest field share resources to keep cross-portal latency low.
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>
namespace fiesta {
class MapClusterManager {
public:
    static MapClusterManager& Get() { static MapClusterManager s; return s; }
    void Register(uint32 uiClusterId, uint16 uiMap) { m_kClusters[uiClusterId].push_back(uiMap); }
private:
    std::map<uint32, std::vector<uint16> > m_kClusters;
};
} // namespace fiesta
