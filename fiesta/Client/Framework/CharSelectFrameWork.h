// Client/Framework/CharSelectFrameWork.h
// Character select framework -- sits between AccountFrameWork and GameFrameWork.
// Receives NC_CHAR_CLIENT_BASE_CMD / NC_CHAR_CLIENT_SHAPE_CMD for the char list,
// lets the player pick or create a character, then hands to GameFrameWork.
//
// Server flow after zone connect:
//   Zone sends NC_CHAR_CLIENT_BASE_CMD  -- one per character slot [handle,name,level,class,...]
//   Zone sends NC_CHAR_CLIENT_SHAPE_CMD -- visual data for each character
//   Player selects → NC_CHAR_LOGIN_REQ  -- standard login (same as direct)
//   Zone responds  → NC_CHAR_LOGIN_ACK + NC_CHAR_INFO_CMD
#ifndef SHINE_CLIENT_FRAMEWORK_CHARSELECTFRAMEWORK_H
#define SHINE_CLIENT_FRAMEWORK_CHARSELECTFRAMEWORK_H

#include "ShineFrameWork.h"
#include "../Network/ZoneSession.h"
#include "../UI/CharSelectUI.h"
#include <vector>
#include <string>

namespace shine {

struct CharSlot {
    uint32      uiCharId;
    std::string kName;
    uint16      uiLevel;
    uint16      uiClass;
    uint16      uiMapId;
    uint8       uiHairType;
    uint8       uiHairColor;
    uint8       uiFaceType;
    bool        bOccupied;

    CharSlot() : uiCharId(0), uiLevel(1), uiClass(0), uiMapId(0),
                 uiHairType(0), uiHairColor(0), uiFaceType(0), bOccupied(false) {}
};

class CharSelectFrameWork : public ShineFrameWork {
public:
    CharSelectFrameWork();

    virtual void OnStart()                           override;
    virtual void OnStop()                            override;
    virtual bool Update(float fDt)                   override;
    virtual void Render()                            override;
    virtual void OnMsg(const FrameWorkMsgData& rMsg) override;

    // Called by ShineApp when NC_CHAR_CLIENT_BASE_CMD / SHAPE arrives
    void OnCharBaseCmd   (const GPacket& rPkt);
    void OnCharShapeCmd  (const GPacket& rPkt);
    void OnCharNewAck    (const GPacket& rPkt);
    void OnCharDelAck    (const GPacket& rPkt);

    // Called by UI when player clicks Enter/Create/Delete
    void SelectChar      (uint32 uiCharId);
    void CreateChar      (const std::string& rName, uint16 uiClass,
                          uint8 uiHairType, uint8 uiHairColor, uint8 uiFaceType);
    void DeleteChar      (uint32 uiCharId);

    void SetZoneSession  (ZoneSession* pkZone) { m_pkZone = pkZone; }
    void SetRenderer     (NiRenderer* pkR)   { m_pkRenderer = pkR; }
    CharSelectUI& GetUI()                    { return m_kUI; }

    const std::vector<CharSlot>& GetSlots() const { return m_kSlots; }

private:
    ZoneSession*            m_pkZone;
    std::vector<CharSlot>   m_kSlots;
    CharSelectUI            m_kUI;
    bool                    m_bListReady;

    NiRenderer*             m_pkRenderer;
};

} // namespace shine
#endif // SHINE_CLIENT_FRAMEWORK_CHARSELECTFRAMEWORK_H
