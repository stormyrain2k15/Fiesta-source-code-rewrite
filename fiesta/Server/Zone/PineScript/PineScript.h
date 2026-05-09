// Server/Zone/PineScript/PineScript.h
// PineScript interpreter core. PineScript is a small server-side scripting
// language used for KQ wave timing, GM events, mob regen, and scripted
// scenes. The engine here parses *.ps files at boot, holds compiled
// programs in memory, and executes them on a frame-driven scheduler.
#ifndef FIESTA_ZONE_PINESCRIPT_H
#define FIESTA_ZONE_PINESCRIPT_H
#include "../../Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace fiesta {

struct PSStatement {
    std::string             kCmd;     // e.g. "MobRegen", "Wait", "Print"
    std::vector<std::string> kArgs;
};

struct PSProgram {
    std::string              kName;
    std::vector<PSStatement> kBody;
    size_t                   uiPC;     // program counter
    uint64                   uiWaitUntilMs;
    bool                     bDone;
};

class PineScript {
public:
    static PineScript& Get();
    bool LoadFile (const std::string& rPath);
    bool LoadDir  (const std::string& rDir);
    bool Run      (const std::string& rName);
    void Tick();
    size_t LoadedCount() const { return m_kPrograms.size(); }
private:
    PineScript() {}
    bool ParseTokens(const std::string& rSrc, PSProgram& rOut);
    bool Step(PSProgram& rProg);
    std::map<std::string, PSProgram> m_kPrograms;     // name -> compiled
    std::vector<PSProgram>           m_kRunning;      // active instances
};

} // namespace fiesta
#endif
