// Server/Shared/well512.h
// WELL512 PRNG (Panneton/L'Ecuyer/Matsumoto).
#ifndef SHINE_WELL512_H
#define SHINE_WELL512_H
#include "ShineTypes.h"

namespace shine {

class well512 {
public:
    well512();
    void     Seed(uint32 uiSeed);
    uint32   Next();                                 // 32-bit
    uint32   NextRange(uint32 uiMaxExclusive);
    double   NextDouble();                           // [0,1)
private:
    uint32 m_auiState[16];
    uint32 m_uiIndex;
};

} // namespace shine
#endif
