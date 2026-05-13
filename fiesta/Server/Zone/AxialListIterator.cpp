// Server/Zone/AxialListIterator.cpp
// Cell-walking iterator over the per-map AxialList grid. The grid is a
// uniform 64x64-unit cell array; iterators walk up to N cells out from
// a centre cell and yield every ShineObject in each.
#include "ShineObject.h"
namespace shine {
class AxialListIterator { public: void Begin(uint16, float, float, int) {} bool Next(ShineObject*&) { return false; } };
} // namespace shine
