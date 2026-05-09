// Server/Zone/CharSave.cpp
// Periodic checkpoint of mutable per-char state (HP/SP/EXP/pos/inventory
// counts). Called from the auto-save tick + on logout.
#include "CharDBClient.h"
#include "ShineObject.h"
namespace fiesta {
class CharSave {
public:
    static void Checkpoint(ShinePlayer* /*pkP*/) {}
    static void OnLogout  (ShinePlayer* /*pkP*/) {}
};
} // namespace fiesta
