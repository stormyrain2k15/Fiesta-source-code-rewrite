// Server/Zone/AttackRhythm.cpp
// Attack timing -- enforces a minimum interval between consecutive
// attacks of the same kind. Backing instance kept anonymous to avoid
// ODR with any existing AttackRhythm declaration.
#include "../Shared/GTimer.h"
#include "../Shared/ShineTypes.h"
#include <map>
namespace fiesta { namespace {
class AttackRhythmImpl {
public:
    static AttackRhythmImpl& Get() { static AttackRhythmImpl s; return s; }
    bool Allow(uint32 cid, uint32 sid, uint32 cdMs) {
        uint64 now = GTimer::NowMillis();
        uint64& last = m_kLast[((uint64)cid << 32) | sid];
        if (now < last + cdMs) return false;
        last = now;
        return true;
    }
private:
    std::map<uint64, uint64> m_kLast;
};
}} // anonymous
