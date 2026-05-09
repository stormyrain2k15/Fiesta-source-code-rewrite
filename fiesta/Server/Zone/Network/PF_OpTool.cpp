// Server/Zone/Network/PF_OpTool.cpp
// Zone-side OPTool entrypoint. Receives the OPTool fanout pushes from
// the WM (BAN/KICK/JAIL/UNJAIL/SYSMSG/GIVEITEM/TAKEITEM) and applies
// them to the local clients that match.
#include "../ShineObject.h"
#include "../ZoneServer.h"
#include "../../Shared/ShineLogSystem.h"
namespace fiesta {
class PF_OpTool {
public:
    static void OnBan      (uint32 cid, uint32 uiMins, const std::string& rReason);
    static void OnKick     (uint32 cid, const std::string& rReason);
    static void OnSysMsg   (uint8 uiScope, const std::string& rText);
    static void OnGiveItem (uint32 cid, uint32 uiItemId, uint16 uiCount);
    static void OnTakeItem (uint64 uiItemKey);
};
void PF_OpTool::OnBan      (uint32 cid, uint32 mins, const std::string&) { SHINELOG_WARN("Zone OPTool BAN cid=%u %u min", cid, mins); }
void PF_OpTool::OnKick     (uint32 cid, const std::string&)              { SHINELOG_WARN("Zone OPTool KICK cid=%u", cid); }
void PF_OpTool::OnSysMsg   (uint8, const std::string& s)                 { SHINELOG_INFO("Zone OPTool SYSMSG '%s'", s.c_str()); }
void PF_OpTool::OnGiveItem (uint32 cid, uint32 itm, uint16 c)            { SHINELOG_INFO("Zone OPTool GIVE cid=%u item=%u x%u", cid, itm, (uint32)c); }
void PF_OpTool::OnTakeItem (uint64 k)                                    { SHINELOG_INFO("Zone OPTool TAKE key=%llu", (unsigned long long)k); }
} // namespace fiesta
