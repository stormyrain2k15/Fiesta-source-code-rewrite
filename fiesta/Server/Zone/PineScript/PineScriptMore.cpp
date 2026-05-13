// Server/Zone/PineScript/PineScriptMore.cpp
// "More" extension commands -- the catch-all bag added late in NA dev for
// scripted broadcasts, weather / time-of-day forcing, and debug helpers.
#include "PineScript.h"
#include "../ZoneServer.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Common/NETCOMMAND.h"
#include "../../Common/SendPacket.h"
#include "../../Shared/PacketBuffer.h"
#include <stdlib.h>

namespace shine {

bool PSCmdMore(const PSStatement& rSt, PSProgram& rProg) {
    if (rSt.kCmd == "Broadcast" && !rSt.kArgs.empty()) {
        // Simple "say to whole zone" -- the zone broadcaster lives in Chat.
        std::string text;
        for (size_t i = 0; i < rSt.kArgs.size(); ++i) { if (i) text += " "; text += rSt.kArgs[i]; }
        PacketBuffer body; body.WriteU8(1); body.WriteString(text);
        // Local-zone fanout: every connected client gets the broadcast.
        const std::map<Handle, ShinePlayer*>& kPlayers = ZoneServer::Get().Players();
        for (std::map<Handle, ShinePlayer*>::const_iterator it = kPlayers.begin();
             it != kPlayers.end(); ++it) {
            if (it->second && it->second->GetSession())
                SendPacket(it->second->GetSession(), NC_CHAT_NORMAL_CMD, body.Data(), body.Size());
        }
        SHINELOG_INFO("PS Broadcast '%s'", text.c_str());
        return true;
    }
    if (rSt.kCmd == "Weather" && !rSt.kArgs.empty()) {
        SHINELOG_INFO("PS Weather=%s", rSt.kArgs[0].c_str());
        return true;
    }
    if (rSt.kCmd == "Sleep" && !rSt.kArgs.empty()) {
        rProg.uiWaitUntilMs = ::atoi(rSt.kArgs[0].c_str());
        return true;
    }
    return false;
}

} // namespace shine
