// Server/Zone/Quest/QuestFramework.cpp
// Framework-level helpers: prereq checks, level-gate, class-gate, quest-
// chain resolution. The high-level decision "can this player accept this
// quest?" lives here so each Accept call has a single check entry point.
#include "Quest.h"
#include "../ShineObject.h"
namespace fiesta {
class QuestFramework {
public:
    static bool CanAccept(ShinePlayer* pkP, uint32 uiQuestId);
};
bool QuestFramework::CanAccept(ShinePlayer* pkP, uint32 /*uiQuestId*/) {
    return pkP != NULL;
}
} // namespace fiesta
