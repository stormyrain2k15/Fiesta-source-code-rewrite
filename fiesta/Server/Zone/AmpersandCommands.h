// Server/Zone/AmpersandCommands.h
#ifndef SHINE_ZONE_AMPCMDS_GEN_H
#define SHINE_ZONE_AMPCMDS_GEN_H
#include "ShineObject.h"
#include <string>
#include <vector>
namespace shine {
struct AmpersandCmdEntry {
    const char* szName;
    uint8       uiMinAdminLv;
    bool        (*pkFn)(ShinePlayer* pk, const std::vector<std::string>& rArgs);
};
extern const AmpersandCmdEntry kAmpersandCmds[];
extern const size_t            kAmpersandCmdCount;
bool DispatchAmpersand(ShinePlayer* pk, const std::string& rLine);
} // namespace shine
#endif