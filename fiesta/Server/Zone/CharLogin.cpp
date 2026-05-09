// Server/Zone/CharLogin.cpp
#include "CharLogin.h"
#include "CharDBClient.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"

namespace fiesta {

void CharLogin(ClientSession* pkSess, const GPacket& rPkt) {
    if (!pkSess) return;
    PacketBuffer b = rPkt.Body();
    AccountID aid = 0; CharID cid = 0;
    uint8 sec[16]; ZeroMemory(sec, 16);
    b.ReadU32(aid); b.ReadU32(cid); b.ReadBytes(sec, 16);
    // Token consumption already happened on WM; Zone trusts WM.
    ShinePlayer* pk = new ShinePlayer();
    pk->FillFromCharLogin(aid, cid, "Adventurer", 1, 0); // provisional until CharDB row arrives
    ZoneServer::Get().AttachPlayer(pk);
    pk->AttachSession(pkSess);

    // Fire the real CharDB lookup. The response handler will call
    // pk->LoadFromCharDBRow asynchronously and overwrite the provisional fill.
    CharDBClient::Get().QueryCharLogin(cid, pk);
    // Pull the player's estate furniture from CharDB so the room is
    // already decorated when they walk through the portal NPC.
    CharDBClient::Get().EstateLoad(cid);

    PacketBuffer ack; ack.WriteU8(1); ack.WriteU32(pk->GetHandle());
    SendPacket(pkSess, NC_CHAR_LOGIN_ACK, ack.Data(), ack.Size());
    SHINELOG_INFO("Zone CharLogin aid=%u cid=%u handle=%u", aid, cid, pk->GetHandle());
}

void CharLogout(ClientSession* pkSess, const GPacket&) {
    if (!pkSess) return;
    if (pkSess->GetPlayer()) {
        CharDBClient::Get().QueryCharLogout(pkSess->GetPlayer()->GetCharID());
    }
    pkSess->Close();
}

void CharMapMarking(ClientSession*, const GPacket&) {
    // 10 Map -- mark the visible block image as discovered (no-op stub).
}

void BriefInfoExchange(ClientSession*, const GPacket&) {
    // 06 -- broadcast brief info for nearby players (no-op stub).
}

} // namespace fiesta
