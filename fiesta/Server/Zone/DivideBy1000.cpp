// Server/Zone/DivideBy1000.cpp
// Tiny helper -- (n + 500) / 1000 with proper rounding. Common pattern
// in scaler arithmetic (e.g. damage * 1234 / 1000).
#include "../Shared/ShineTypes.h"
namespace shine { static inline int32 DivideBy1000(int32 n) { return (n >= 0) ? (n + 500) / 1000 : (n - 500) / 1000; } }
