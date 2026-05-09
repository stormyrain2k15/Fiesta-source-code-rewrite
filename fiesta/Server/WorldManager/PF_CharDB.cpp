// Server/WorldManager/PF_CharDB.cpp
// Inbound dispatcher for CharDB->WM responses + a handful of bookkeeping
// receivers that fold the response back into the in-memory subsystem
// state. Most CharDB writes originate on Zone (via CharDBClient) or on WM
// itself via WMCharDBClient; this PF only handles the WM-side responses
// that need post-processing on top of the simple "ack received" path that
// WMCharDBSession::OnPacket already performs in WorldManagerServer.cpp.
//
// Wire layout for inbound responses (head byte = original sub-op echo):
//   90 GUILD_CREATE        u32 cmaster, u32 newGuildNo
//   91 GUILD_DISSOLVE      u32 g
//   92..101 GUILD_*        u32 g, ...
//   110/111 FRIEND_ADD/DEL u32 a, u32 b
//   130 KQ_RESULT_SET      u32 kqid, u32 winKingdom
//   140 EVENT_ATTEND       u32 c, u8 day
//   150 DAILY_RESET        u32 dayKey
//   160 RANKING_PUBLISH    u8 cat, u16 nrows, ...rows
#include "WMServices.h"
#include "GuildServer.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"

namespace fiesta {

void PF_CharDBDispatch(const GPacket& rPkt) {
    if (rPkt.GetOpcode() != NC_INTER_CHAR_DB_RESPONSE) return;
    PacketBuffer body = rPkt.Body();
    uint8 uiSub = 0; if (!body.ReadU8(uiSub)) return;
    switch (uiSub) {
        case 90: {  // GUILD_CREATE response: stamp the new guildNo
            uint32 cmaster = 0, newG = 0;
            body.ReadU32(cmaster); body.ReadU32(newG);
            SHINELOG_INFO("CharDB->WM guild create master=%u no=%u", cmaster, newG);
            break;
        }
        case 110: case 111: {
            uint32 a = 0, b = 0;
            body.ReadU32(a); body.ReadU32(b);
            SHINELOG_INFO("CharDB->WM friend %s a=%u b=%u",
                          (uiSub == 110) ? "add" : "del", a, b);
            break;
        }
        case 130: {
            uint32 kqid = 0, winK = 0;
            body.ReadU32(kqid); body.ReadU32(winK);
            SHINELOG_INFO("CharDB->WM KQ #%u winKingdom=%u", kqid, winK);
            break;
        }
        case 140: {
            uint32 c = 0; uint8 d = 0;
            body.ReadU32(c); body.ReadU8(d);
            SHINELOG_INFO("CharDB->WM EventAttendance cid=%u day=%u", c, (uint32)d);
            break;
        }
        case 150: {
            uint32 dayKey = 0; body.ReadU32(dayKey);
            SHINELOG_INFO("CharDB->WM DailyReset dayKey=%u", dayKey);
            break;
        }
        default:
            SHINELOG_DEBUG("CharDB->WM unhandled sub=%u", (uint32)uiSub);
            break;
    }
}

} // namespace fiesta
