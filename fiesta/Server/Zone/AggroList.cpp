// Server/Zone/AggroList.cpp
#include "AggroList.h"

namespace shine {

AggroList::Entry* AggroList::Find(CharID c) {
    for (size_t i = 0; i < m_kEntries.size(); ++i) {
        if (m_kEntries[i].c == c) return &m_kEntries[i];
    }
    return NULL;
}

void AggroList::Add(CharID c, int32 iAmount) {
    if (c == 0) return;
    Entry* p = Find(c);
    if (p) {
        // saturating add at int32 max
        int64 v = (int64)p->iHate + iAmount;
        if (v > (int64)0x7FFFFFFFLL) v = (int64)0x7FFFFFFFLL;
        if (v < 0) v = 0;
        p->iHate = (int32)v;
    } else {
        Entry e; e.c = c; e.iHate = iAmount < 0 ? 0 : iAmount;
        m_kEntries.push_back(e);
    }
}

void AggroList::Set(CharID c, int32 iAmount) {
    if (c == 0) return;
    Entry* p = Find(c);
    if (p) p->iHate = iAmount;
    else { Entry e; e.c = c; e.iHate = iAmount; m_kEntries.push_back(e); }
}

void AggroList::Drop(CharID c) {
    for (size_t i = 0; i < m_kEntries.size(); ++i) {
        if (m_kEntries[i].c == c) {
            m_kEntries.erase(m_kEntries.begin() + i);
            return;
        }
    }
}

CharID AggroList::Top() const {
    int32 best = -1; CharID bestId = 0;
    for (size_t i = 0; i < m_kEntries.size(); ++i) {
        if (m_kEntries[i].iHate > best) {
            best = m_kEntries[i].iHate;
            bestId = m_kEntries[i].c;
        }
    }
    return bestId;
}

void AggroList::Decay(int32 iAmount) {
    if (iAmount <= 0) return;
    size_t w = 0;
    for (size_t i = 0; i < m_kEntries.size(); ++i) {
        m_kEntries[i].iHate -= iAmount;
        if (m_kEntries[i].iHate > 0) {
            if (w != i) m_kEntries[w] = m_kEntries[i];
            ++w;
        }
    }
    m_kEntries.resize(w);
}

int32 AggroList::LevelGapMulX1k(uint16 uiPlayerLevel, uint16 uiMobLevel) {
    if ((int32)uiPlayerLevel <= (int32)uiMobLevel) return 1000;
    int32 over = (int32)uiPlayerLevel - (int32)uiMobLevel;
    if (over >= kAggroLevelSpan)                    return 0;
    // Linear fade across the 20-level span.
    return ((kAggroLevelSpan - over) * 1000) / kAggroLevelSpan;
}

void AggroList::AddScaled(CharID c, int32 iRawHate, uint16 uiPlayerLevel, uint16 uiMobLevel) {
    int32 mul = LevelGapMulX1k(uiPlayerLevel, uiMobLevel);
    if (mul <= 0) return;       // out of span -- mob ignores this attacker
    int64 scaled = (int64)iRawHate * (int64)mul / 1000;
    if (scaled > (int64)0x7FFFFFFFLL) scaled = (int64)0x7FFFFFFFLL;
    Add(c, (int32)scaled);
}

} // namespace shine
