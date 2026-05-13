// Client/Engine/ShineApp.h
#ifndef SHINE_CLIENT_ENGINE_SHINEAPP_H
#define SHINE_CLIENT_ENGINE_SHINEAPP_H

#include <NiApplication.h>
#include <NiMain.h>
#include <NiAnimation.h>
#include <NiDX9Renderer.h>

#include "../../Server/Shared/ShineTypes.h"
#include "../Framework/ShineFrameWork.h"
#include "../Framework/AccountFrameWork.h"
#include "../Framework/CharSelectFrameWork.h"
#include "../Framework/GameFrameWork.h"
#include "../Network/ZoneSession.h"
#include <string>

namespace shine {

class ShineApp : public NiApplication {
    NiDeclareRTTI;
public:
    static ShineApp* ms_pkApp;
    static ShineApp* GetInstance() { return ms_pkApp; }

    ShineApp();
    virtual ~ShineApp();

    virtual bool    Initialize()                        override;
    virtual bool    UpdateFrame()                       override;
    virtual bool    RenderFrame()                       override;
    virtual void    Terminate()                         override;
    virtual bool    CreateRenderer()                    override;
    virtual bool    OnDefault(NiEventRef pEventRecord)  override;

    // Render-thread safe -- called after net event pump
    void OnZoneReady         (const PlayerState& rPlayer);
    void OnGamePacket        (const GPacket& rPkt);
    void OnCharBaseCmd       (const GPacket& rPkt);
    void OnCharShapeCmd      (const GPacket& rPkt);

    void SetZoneSession      (ZoneSession* pkZone);

    // CharSelectUI handlers (routed via static thunks below).
    void OnCharSelected(uint32 uiCharId);
    void OnCharCreated (const std::string& rName, uint16 uiClass,
                        uint8 uiHair, uint8 uiHairColor, uint8 uiFace);
    void OnCharDeleted (uint32 uiCharId);

    GameFrameWork*       GetGameFW()      { return &m_kGameFW; }
    AccountFrameWork*    GetAccountFW()   { return &m_kAccountFW; }
    CharSelectFrameWork* GetCharSelectFW(){ return &m_kCharSelectFW; }

private:
    AccountFrameWork    m_kAccountFW;
    CharSelectFrameWork m_kCharSelectFW;
    GameFrameWork       m_kGameFW;
    ZoneSession*        m_pkZone;

    // Zone-ready handoff (net thread -> render thread).
    // VS2010 has no <atomic>; use volatile LONG + InterlockedExchange.
    volatile LONG       m_lZoneReady;
    PlayerState         m_kPendingPlayer;
    CRITICAL_SECTION    m_kStateLock;

    // Char base/shape handoff
    volatile LONG       m_lCharBase;
    volatile LONG       m_lCharShape;
    GPacket             m_kPendingCharBase;
    GPacket             m_kPendingCharShape;

    DWORD               m_dwLastTick;

    float GetDeltaTime();
    void  PumpPendingEvents();

    // Static thunks routing CharSelectUI's C-style callbacks back here.
    static void CharSelectThunk(void* pkCtx, uint32 uiCharId);
    static void CharCreateThunk(void* pkCtx, const std::string& rName,
                                 uint16 uiClass, uint8 uiHair,
                                 uint8 uiHairColor, uint8 uiFace);
    static void CharDeleteThunk(void* pkCtx, uint32 uiCharId);
};

} // namespace shine
#endif // SHINE_CLIENT_ENGINE_SHINEAPP_H
