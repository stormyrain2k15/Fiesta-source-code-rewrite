// Server/Zone/Quest/ShineQuestDiary.cpp
// Quest diary / journal -- per-player log of currently-active and
// historical quests. UI renders this; the diary itself is just an index
// over Quest.cpp's per-char map.
#include "Quest.h"
namespace shine {
class ShineQuestDiary {
public:
    static void RebuildFor(uint32 /*cid*/) {}
};
} // namespace shine
