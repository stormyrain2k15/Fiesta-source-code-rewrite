// Server/Zone/MobTraceStack.cpp
// Mob movement-history stack -- last-N positions used for chase
// path-back / stuck-detect.
#include "../Shared/ShineTypes.h"
#include <vector>
namespace shine {
struct TracePos { float x, y; uint64 uiAt; };
class MobTraceStack {
public:
    void Push(float x, float y, uint64 t) { TracePos p={x,y,t}; m_kStack.push_back(p); if (m_kStack.size()>16) m_kStack.erase(m_kStack.begin()); }
    bool IsStuck() const { return m_kStack.size() > 4 && m_kStack.front().x == m_kStack.back().x && m_kStack.front().y == m_kStack.back().y; }
private:
    std::vector<TracePos> m_kStack;
};
} // namespace shine
