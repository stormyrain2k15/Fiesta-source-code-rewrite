// Server/Zone/ShineItemRepurchase.cpp
// Per-session repurchase buffer -- last N items sold to an NPC, buyable
// back at the original price within the same session. Cleared on logout.
#include "Inventory.h"
#include <map>
#include <vector>
namespace fiesta {
struct RepurchaseEntry { uint32 uiItemId; uint16 uiCount; uint32 uiUnitPrice; };
class ShineItemRepurchase {
public:
    static ShineItemRepurchase& Get() { static ShineItemRepurchase s; return s; }
    void Push (uint32 cid, const RepurchaseEntry& e) { m_kBuf[cid].push_back(e); }
    void Clear(uint32 cid) { m_kBuf.erase(cid); }
private:
    std::map<uint32, std::vector<RepurchaseEntry> > m_kBuf;
};
} // namespace fiesta
