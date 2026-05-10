#include "InstanceDungeon.h"
#include "ShineObject.h"
#include "ZoneServer.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/GTimer.h"
namespace fiesta {

MIDServer& MIDServer::Get() { static MIDServer s; return s; }

uint32 MIDServer::SpinUp(uint32 t, const std::vector<CharID>& g) {
    MIDGroupRec r; r.uiInstanceId = m_uiNextId++; r.kMembers = g;
    r.uiStartMs = GTimer::NowMillis();
    r.uiEndMs   = r.uiStartMs + InstanceDungeon_util::ProvisionalDurationMs(t);
    m_kActive[r.uiInstanceId] = r;
    return r.uiInstanceId;
}
void MIDServer::TearDown(uint32 id) { m_kActive.erase(id); }
bool MIDServer::IsActive(uint32 id) const { return m_kActive.find(id) != m_kActive.end(); }
void MIDServer::Tick() {
    uint64 now = GTimer::NowMillis();
    for (std::map<uint32, MIDGroupRec>::iterator it = m_kActive.begin(); it != m_kActive.end(); ) {
        if (now > it->second.uiEndMs) { std::map<uint32, MIDGroupRec>::iterator k = it++; m_kActive.erase(k); }
        else ++it;
    }
}

int32 MIDRewardDataBox::GoldFor(uint32, uint16 clr) {
    int32 base = 1000;
    if (clr < 60)  return base * 3;
    if (clr < 120) return base * 2;
    return base;
}
bool  MatchInstanceDungeonServer::TryQueue(ShinePlayer* pk, uint32) { return pk != NULL; }
void InstanceDungeon::OnPlayerEnter(ShinePlayer* pk, uint32 uiTypeId) {
    if (!pk) return;
    // Warp player to instance entry point.
    // Entry coordinates are PROVISIONAL — sourced from KQInfo.shn when wired.
    // For now, zero-origin so the zone at least places them on the map.
    // TUNE: load from KQInfo/InstanceInfo SHN once the column RE is confirmed.
    Vec3 entryPos(0.0f, 0.0f, 0.0f);
    pk->SetPos(entryPos);
    SHINELOG_INFO("InstanceDungeon::OnPlayerEnter pk=%u type=%u",
                  pk->GetHandle(), uiTypeId);
    // Broadcast the player's new position (NC_ACT_STOP_CMD is the minimal
    // "here I am" packet the client accepts on zone transfer).
    if (pk->GetSession()) {
        PacketBuffer pkt;
        pkt.WriteF32(entryPos.x);
        pkt.WriteF32(entryPos.y);
        pkt.WriteF32(entryPos.z);
        SendPacket(pk->GetSession(), NC_ACT_STOP_CMD, pkt.Data(), pkt.Size());
    }
}
uint64 InstanceDungeon_util::ProvisionalDurationMs(uint32) { return 30ULL * 60ULL * 1000ULL; }

} // namespace fiesta
