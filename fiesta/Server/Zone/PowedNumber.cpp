// Server/Zone/PowedNumber.cpp
// Integer power helper -- iterative pow for VS2010 ints.
#include "../Shared/ShineTypes.h"
namespace shine {
static inline uint64 PowedNumber(uint32 uiBase, uint32 uiExp) {
    uint64 r = 1; for (uint32 i = 0; i < uiExp; ++i) r *= uiBase; return r;
}
}
