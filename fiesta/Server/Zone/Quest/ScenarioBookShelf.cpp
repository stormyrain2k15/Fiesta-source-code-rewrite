// Server/Zone/Quest/ScenarioBookShelf.cpp
// "Scenario book" -- collection of PineScript programs grouped by chapter.
// At zone boot the bookshelf walks Data/Scenario/ and registers every
// script with the PineScript engine. Acts as the discovery layer.
#include "../PineScript/PineScript.h"
namespace shine {
class ScenarioBookShelf {
public:
    static bool LoadAll(const std::string& rDir) { return PineScript::Get().LoadDir(rDir); }
};
} // namespace shine
