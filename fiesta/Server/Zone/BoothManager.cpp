// Server/Zone/BoothManager.cpp
// Per-zone booth registry -- live list of open booths for proximity
// search and for the global booth-search UI.
#include "../Shared/ShineTypes.h"
#include <map>
namespace fiesta { namespace {
class BoothManager {
public:
    static BoothManager& Get() { static BoothManager s; return s; }
    void OnOpen (uint32 cid) { ++m_kOpen[cid]; }
    void OnClose(uint32 cid) { m_kOpen.erase(cid); }
private:
    std::map<uint32, uint32> m_kOpen;
};
}} // anonymous
