// Server/Zone/InventoryLocking.cpp
// Per-slot lock during trade / booth listing. Locked items can't be
// dropped, sold, or moved until the operation completes / aborts.
#include "../Shared/ShineTypes.h"
#include <set>
namespace fiesta {
class InventoryLocking {
public:
    static InventoryLocking& Get() { static InventoryLocking s; return s; }
    void Lock  (uint32 cid, uint16 uiSlot) { m_kLocked.insert(((uint64)cid << 16) | uiSlot); }
    void Unlock(uint32 cid, uint16 uiSlot) { m_kLocked.erase (((uint64)cid << 16) | uiSlot); }
    bool IsLocked(uint32 cid, uint16 uiSlot) const {
        return m_kLocked.count(((uint64)cid << 16) | uiSlot) > 0;
    }
private:
    std::set<uint64> m_kLocked;
};
} // namespace fiesta
