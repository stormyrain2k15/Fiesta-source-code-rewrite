// Server/Zone/Tables/SetEffectTable.cpp
// FEATURE: world-creation -- SetEffect.shn binder. Set-bonus rows
// (SetItemIndex, Count) -> action id triggered when N pieces equipped.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace shine {

SetEffectTable& SetEffectTable::Get() { static SetEffectTable s; return s; }

void SetEffectTable::Bind() {
    // FEATURE: world-creation -- column read: SetItemIndex, Count, ItemActionID
    BIND_BEGIN(t, "SetEffect")
    ITER_ROWS(t) {
        Row r;
        r.uiSetIndex = ShnGetU32(*t, _r, "SetItemIndex");
        r.uiCount    = ShnGetU32(*t, _r, "Count");
        r.uiActionID = ShnGetU32(*t, _r, "ItemActionID");
        m_kBySet[r.uiSetIndex].push_back(m_kRows.size());
        m_kRows.push_back(r);
    }
}

void SetEffectTable::RowsForSet(uint32 uiIdx, std::vector<const Row*>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<size_t> >::const_iterator it = m_kBySet.find(uiIdx);
    if (it == m_kBySet.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i)
        rOut.push_back(&m_kRows[it->second[i]]);
}

} // namespace shine
