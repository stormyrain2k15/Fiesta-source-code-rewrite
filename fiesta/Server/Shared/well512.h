// Server/Shared/well512.h
// 01 Foundation -- WELL512 PRNG (Panneton/L'Ecuyer/Matsumoto).
// EVIDENCE: PDB_CONFIRMED  symbol: well512
#ifndef FIESTA_WELL512_H
#define FIESTA_WELL512_H
#include "ShineTypes.h"

namespace fiesta {

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

} // namespace fiesta
#endif
