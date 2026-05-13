// Server/Zone/PineScript/PineScriptFunction.cpp
// Built-in command table for the PineScript interpreter. Each command
// returns true if recognised so the interpreter can fall through to the
// "unknown" warning otherwise. Add new commands here, not in PineScript.cpp.
#include "PineScript.h"
#include "../../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace shine {

// Forward decls from sibling files.
extern bool PSCmdMobRegen (const PSStatement&, PSProgram&);
extern bool PSCmdMore     (const PSStatement&, PSProgram&);

bool PSDispatchFunction(const PSStatement& rSt, PSProgram& rProg) {
    if (rSt.kCmd == "Print") {
        std::string msg;
        for (size_t i = 0; i < rSt.kArgs.size(); ++i) { if (i) msg += " "; msg += rSt.kArgs[i]; }
        SHINELOG_INFO("PS:%s -> %s", rProg.kName.c_str(), msg.c_str());
        return true;
    }
    if (rSt.kCmd == "Goto" && !rSt.kArgs.empty()) {
        // Numeric program-counter jump (label resolution is intentionally
        // skipped -- PineScript labels are stripped at parse).
        rProg.uiPC = (size_t)atoi(rSt.kArgs[0].c_str());
        return true;
    }
    if (rSt.kCmd == "End") { rProg.bDone = true; return true; }

    if (PSCmdMobRegen(rSt, rProg)) return true;
    if (PSCmdMore    (rSt, rProg)) return true;
    return false;
}

} // namespace shine
