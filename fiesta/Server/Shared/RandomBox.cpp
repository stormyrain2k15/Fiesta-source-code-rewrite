// Server/Shared/RandomBox.cpp
#include "RandomBox.h"

namespace shine {

RandomBox::RandomBox() : m_uiTotal(0) {}

void RandomBox::Clear() { m_kSlots.clear(); m_uiTotal = 0; }

void RandomBox::AddSlot(uint32 uiId, uint32 uiWeight) {
    if (uiWeight == 0) return;
    Slot s; s.uiId = uiId; s.uiWeight = uiWeight;
    m_kSlots.push_back(s);
    m_uiTotal += uiWeight;
}

uint32 RandomBox::Draw() {
    if (m_uiTotal == 0 || m_kSlots.empty()) return 0;
    uint32 r = m_kRng.NextRange(m_uiTotal);
    uint32 acc = 0;
    for (size_t i = 0; i < m_kSlots.size(); ++i) {
        acc += m_kSlots[i].uiWeight;
        if (r < acc) return m_kSlots[i].uiId;
    }
    return m_kSlots.back().uiId;
}

} // namespace shine
