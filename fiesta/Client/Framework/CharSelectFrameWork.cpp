// Client/Framework/CharSelectFrameWork.cpp
#include "CharSelectFrameWork.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../Engine/ShineConfig.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

namespace shine {

CharSelectFrameWork::CharSelectFrameWork()
    : m_pkZone(NULL), m_bListReady(false), m_pkRenderer(NULL)
{}

void CharSelectFrameWork::OnStart() {
    m_kSlots.clear();
    m_bListReady = false;
    SHINELOG_INFO("CharSelectFrameWork: waiting for char list");
}

void CharSelectFrameWork::OnStop() {
    m_kUI.Shutdown();
    SHINELOG_INFO("CharSelectFrameWork: stopped");
}

bool CharSelectFrameWork::Update(float fDt) {
    m_kUI.Update(fDt);
    return true;
}

void CharSelectFrameWork::Render() {
    m_kUI.Render(m_pkRenderer);
}

void CharSelectFrameWork::OnMsg(const FrameWorkMsgData& rMsg) {
    switch (rMsg.eMsg) {
    case PGFM_EXIT:
    case PGFM_DISCONNECT:
        if (m_pkZone) m_pkZone->Disconnect();
        Pgg_kFrameMgr.Start(AccountFrameWorkID);
        break;
    default: break;
    }
}

// ── NC packet handlers ────────────────────────────────────────────────────────

void CharSelectFrameWork::OnCharBaseCmd(const GPacket& rPkt) {
    // NC_CHAR_CLIENT_BASE_CMD:
    // [uint8 slot][uint32 charId][string name][uint16 level][uint16 class]
    // [uint16 mapId][int32 hp][int32 maxHp][int32 sp][int32 maxSp]
    PacketBuffer body = rPkt.Body();
    CharSlot slot;
    uint8 uiSlotIdx = 0;
    body.ReadU8(uiSlotIdx);
    body.ReadU32(slot.uiCharId);
    body.ReadString(slot.kName);
    body.ReadU16(slot.uiLevel);
    body.ReadU16(slot.uiClass);
    body.ReadU16(slot.uiMapId);
    slot.bOccupied = (slot.uiCharId != 0);

    // Expand slots vector if needed
    while ((size_t)uiSlotIdx >= m_kSlots.size())
        m_kSlots.push_back(CharSlot());
    m_kSlots[uiSlotIdx] = slot;

    SHINELOG_INFO("CharSelectFW: slot %u -> '%s' lv=%u class=%u",
                  uiSlotIdx, slot.kName.c_str(), slot.uiLevel, slot.uiClass);
}

void CharSelectFrameWork::OnCharShapeCmd(const GPacket& rPkt) {
    // NC_CHAR_CLIENT_SHAPE_CMD:
    // [uint8 slot][uint8 hairType][uint8 hairColor][uint8 faceType]
    PacketBuffer body = rPkt.Body();
    uint8 uiSlotIdx=0, uiHair=0, uiHairColor=0, uiFace=0;
    body.ReadU8(uiSlotIdx);
    body.ReadU8(uiHair);
    body.ReadU8(uiHairColor);
    body.ReadU8(uiFace);

    if ((size_t)uiSlotIdx < m_kSlots.size()) {
        m_kSlots[uiSlotIdx].uiHairType  = uiHair;
        m_kSlots[uiSlotIdx].uiHairColor = uiHairColor;
        m_kSlots[uiSlotIdx].uiFaceType  = uiFace;
    }

    // Once shape data arrives for the last slot, list is ready
    m_bListReady = true;
    if (!m_kUI.IsInitialized() && m_pkRenderer) {
        m_kUI.Init(m_pkRenderer, m_kSlots);
    }
}

void CharSelectFrameWork::OnCharNewAck(const GPacket& rPkt) {
    // NC_CHAR_NEW_ACK: [uint8 result][uint8 slot][uint32 charId]
    PacketBuffer body = rPkt.Body();
    uint8 uiResult=0, uiSlot=0; uint32 uiCharId=0;
    body.ReadU8(uiResult);
    body.ReadU8(uiSlot);
    body.ReadU32(uiCharId);
    if (uiResult == 0) {
        SHINELOG_INFO("CharSelectFW: new char created id=%u slot=%u", uiCharId, uiSlot);
        // Server will re-send NC_CHAR_CLIENT_BASE_CMD for the new slot
        m_kUI.ShowNotice("Character created!");
    } else {
        SHINELOG_WARN("CharSelectFW: char create failed result=%u", uiResult);
        m_kUI.ShowNotice("Create failed. Name may be taken.");
    }
}

void CharSelectFrameWork::OnCharDelAck(const GPacket& rPkt) {
    // NC_CHAR_DEL_ACK: [uint8 result][uint8 slot]
    PacketBuffer body = rPkt.Body();
    uint8 uiResult=0, uiSlot=0;
    body.ReadU8(uiResult);
    body.ReadU8(uiSlot);
    if (uiResult == 0) {
        if ((size_t)uiSlot < m_kSlots.size())
            m_kSlots[uiSlot] = CharSlot(); // clear slot
        m_kUI.Refresh(m_kSlots);
        SHINELOG_INFO("CharSelectFW: char deleted slot=%u", uiSlot);
    } else {
        m_kUI.ShowNotice("Delete failed.");
    }
}

// ── Actions triggered by UI ───────────────────────────────────────────────────

void CharSelectFrameWork::SelectChar(uint32 uiCharId) {
    if (!m_pkZone) return;
    // NC_CHAR_LOGIN_REQ sent by ZoneSession -- just need to update charId
    // The ZoneSession was already connected; we send a new login for the selected char
    PacketBuffer body;
    body.WriteU32(uiCharId);
    m_pkZone->Net().SendPacket(NC_CHAR_LOGIN_REQ, body.Data(), body.Size());
    SHINELOG_INFO("CharSelectFW: selected char %u", uiCharId);
}

void CharSelectFrameWork::CreateChar(const std::string& rName, uint16 uiClass,
                                      uint8 uiHairType, uint8 uiHairColor, uint8 uiFaceType) {
    if (!m_pkZone) return;
    // NC_CHAR_NEW_REQ: [string name][uint16 class][uint8 hair][uint8 hairColor][uint8 face]
    PacketBuffer body;
    body.WriteString(rName);
    body.WriteU16(uiClass);
    body.WriteU8(uiHairType);
    body.WriteU8(uiHairColor);
    body.WriteU8(uiFaceType);
    m_pkZone->Net().SendPacket(NC_CHAR_NEW_REQ, body.Data(), body.Size());
    SHINELOG_INFO("CharSelectFW: create '%s' class=%u", rName.c_str(), uiClass);
}

void CharSelectFrameWork::DeleteChar(uint32 uiCharId) {
    if (!m_pkZone) return;
    // NC_CHAR_DEL_REQ: [uint32 charId]
    PacketBuffer body;
    body.WriteU32(uiCharId);
    m_pkZone->Net().SendPacket(NC_CHAR_DEL_REQ, body.Data(), body.Size());
    SHINELOG_INFO("CharSelectFW: delete char %u", uiCharId);
}

} // namespace shine
