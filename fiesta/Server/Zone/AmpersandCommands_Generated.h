// Server/Zone/AmpersandCommands_Generated.h
// EVIDENCE: DATA_CONFIRMED  source: gm_commands_index.csv (155 commands)
#ifndef FIESTA_ZONE_AMPCMDS_GEN_H
#define FIESTA_ZONE_AMPCMDS_GEN_H
#include "ShineObject.h"
#include <string>
#include <vector>
namespace fiesta {
struct AmpersandCmdEntry {
    const char* szName;
    uint8       uiMinAdminLv;
    bool        (*pkFn)(ShinePlayer* pk, const std::vector<std::string>& rArgs);
};
extern const AmpersandCmdEntry kAmpersandCmds[];
extern const size_t            kAmpersandCmdCount;
bool DispatchAmpersand(ShinePlayer* pk, const std::string& rLine);
} // namespace fiesta
#endif