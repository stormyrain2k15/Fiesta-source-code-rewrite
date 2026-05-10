// Server/Zone/CharLogin.cpp
#include "CharLogin.h"
#include "CharDBClient.h"
#include "TypedSchemaConsumers.h"
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

    // Real CharDB lookup. The response handler calls LoadFromCharDBRow
    // asynchronously and overwrites the provisional fill above.
    CharDBClient::Get().QueryCharLogin(cid, pk);
    // Pull the player's estate furniture from CharDB so the room is
    // already decorated when they walk through the portal NPC.
    CharDBClient::Get().EstateLoad(cid);

    // StateField auto-effects: if the LoginZone the player landed on has
    // an entry in StateField.shn, apply the bound AbState immediately so
    // the world's environmental effect (heat, freeze, ...) is on from
    // frame zero.
    StateFieldTable::Get().OnPlayerEnter(pk, pk->GetLoginZone());

    // NC_CHAR_LOGIN_ACK: [ok(1), handle(4)]
    PacketBuffer ack;
    ack.WriteU8(1);
    ack.WriteU32(pk->GetHandle());
    SendPacket(pkSess, NC_CHAR_LOGIN_ACK, ack.Data(), ack.Size());

    // NC_CHAR_INFO_CMD — send the client its own character state so the
    // UI can render the correct HP/SP/level/class/position immediately.
    // PROVISIONAL packet shape: verify field order against a real capture.
    // This is the minimum viable set for phase-1: client needs level + HP
    // to display the character on screen.
    // [handle(4), level(2), class(2), hp(4), maxhp(4), sp(4), maxsp(4),
    //  posX(4f), posY(4f), posZ(4f), mapId(2)]
    {
        PacketBuffer info;
        info.WriteU32(pk->GetHandle());
        info.WriteU16(pk->GetLevel());
        info.WriteU16(pk->GetClass());
        info.WriteI32(pk->GetHP());
        info.WriteI32(pk->GetMaxHP());
        info.WriteI32(pk->GetSP());
        info.WriteI32(pk->GetMaxSP());
        const Vec3& pos = pk->GetPos();
        info.WriteF32(pos.x);
        info.WriteF32(pos.y);
        info.WriteF32(pos.z);
        info.WriteU16(pk->GetMap());
        SendPacket(pkSess, NC_CHAR_INFO_CMD, info.Data(), info.Size());
    }

    SHINELOG_INFO("Zone CharLogin aid=%u cid=%u handle=%u lv=%u",
                  aid, cid, pk->GetHandle(), pk->GetLevel());
}

void CharLogout(ClientSession* pkSess, const GPacket&) {
    if (!pkSess) return;
    if (pkSess->GetPlayer()) {
        CharDBClient::Get().QueryCharLogout(pkSess->GetPlayer()->GetCharID());
    }
    pkSess->Close();
}

void CharMapMarking(ClientSession*, const GPacket&) {
    // 10 Map -- mark the visible block image as discovered.
}

void BriefInfoExchange(ClientSession*, const GPacket&) {
    // 06 -- broadcast brief info for nearby players.
}

} // namespace fiesta
