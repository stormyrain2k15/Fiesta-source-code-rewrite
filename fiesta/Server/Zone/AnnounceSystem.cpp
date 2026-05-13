// Server/Zone/AnnounceSystem.cpp
// Zone-wide announcement helper -- system messages, broadcasts from
// PineScript / GMEvent / KQ.
#include "AnnounceSystem.h"
#include "ZoneServer.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../Shared/PacketBuffer.h"

namespace shine {

void AnnounceSystem::Broadcast(uint8 uiLevel, const std::string& rText) {
    PacketBuffer body;
    body.WriteU8(uiLevel);
    body.WriteString(rText);
    const std::map<Handle, ShinePlayer*>& kAll = ZoneServer::Get().Players();
    for (std::map<Handle, ShinePlayer*>::const_iterator it = kAll.begin();
         it != kAll.end(); ++it) {
        if (it->second && it->second->GetSession())
            SendPacket(it->second->GetSession(), NC_CHAT_NORMAL_CMD,
                       body.Data(), body.Size());
    }
}

} // namespace shine
