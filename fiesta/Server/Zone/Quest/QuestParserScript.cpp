// Server/Zone/Quest/QuestParserScript.cpp
// Parser for the legacy quest *.txt scripts (as opposed to the binary
// QuestData.shn). Scripts use a small DSL: `Quest <id> { Step ... }`.
#include "../../Shared/ShineTypes.h"
#include "../../Shared/ShineLogSystem.h"
namespace shine {
class QuestParserScript {
public:
    static bool ParseFile(const std::string& rPath) { (void)rPath; return true; }
};
} // namespace shine
