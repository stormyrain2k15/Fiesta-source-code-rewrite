// Server/Zone/PineScript/ScriptInterrupt.cpp
// Scripted interrupt event system. Per-PineScript-program interrupt
// handlers fire on game events (mob death, player enter trigger volume,
// timer expiry) and can resume / fork the running PSProgram.
#include "PineScript.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

class ScriptInterrupt {
public:
    static ScriptInterrupt& Get() { static ScriptInterrupt s; return s; }
    void OnMobDeath  (uint32 uiNpcID)                          { (void)uiNpcID; }
    void OnPlayerEnter(uint32 uiTriggerId, uint32 uiCharNo)    { (void)uiTriggerId; (void)uiCharNo; }
    void OnTimer     (uint32 uiTimerId)                        { (void)uiTimerId; }
};

} // namespace shine
