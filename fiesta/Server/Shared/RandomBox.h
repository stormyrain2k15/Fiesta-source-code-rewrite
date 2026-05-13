// Server/Shared/RandomBox.h
// weighted random / dice helpers used by Drop / Belong / Gamble.
#ifndef SHINE_RANDOMBOX_H
#define SHINE_RANDOMBOX_H
#include "well512.h"
#include <vector>

namespace shine {

class RandomBox {
public:
    RandomBox();
    void   Clear();
    void   AddSlot(uint32 uiId, uint32 uiWeight);
    uint32 Draw();                                   // returns slot id, 0 if empty
    uint32 GetWeightTotal() const { return m_uiTotal; }
private:
    struct Slot { uint32 uiId; uint32 uiWeight; };
    std::vector<Slot> m_kSlots;
    uint32            m_uiTotal;
    well512           m_kRng;
};

} // namespace shine
#endif
