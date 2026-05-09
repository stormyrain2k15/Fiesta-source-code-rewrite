// Server/Zone/ItemActionObserve.cpp
// Observer pattern over item actions for telemetry / titles / quest
// progression. Observers register here; effect dispatcher fires them on
// successful apply.
#include "../Shared/ShineTypes.h"
#include <vector>
namespace fiesta {
typedef void (*ItemActionObserverFn)(uint32 cid, uint32 uiItemId, uint32 uiActionId);
class ItemActionObserve {
public:
    static ItemActionObserve& Get() { static ItemActionObserve s; return s; }
    void Register(ItemActionObserverFn pkFn) { m_kObservers.push_back(pkFn); }
    void Notify(uint32 cid, uint32 uiItemId, uint32 uiActionId) {
        for (size_t i = 0; i < m_kObservers.size(); ++i) m_kObservers[i](cid, uiItemId, uiActionId);
    }
private:
    std::vector<ItemActionObserverFn> m_kObservers;
};
} // namespace fiesta
