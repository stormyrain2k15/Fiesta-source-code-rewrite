// Client/Framework/ShineFrameWork.h
#ifndef SHINE_CLIENT_FRAMEWORK_SHINEFRAMEWORK_H
#define SHINE_CLIENT_FRAMEWORK_SHINEFRAMEWORK_H
#include "../../Server/Shared/ShineTypes.h"
#include <map>
namespace shine {

enum FrameWorkID {
    AccountFrameWorkID   = 0,
    CharSelectFrameWorkID= 1,
    GameFrameWorkID      = 2,
};

enum FrameWorkMsg {
    PGFM_NONE=0, PGFM_EXIT=1, PGFM_DISCONNECT=2,
    PGFM_ZONE_READY=3, PGFM_LOGIN_SUCCESS=4, PGFM_LOGIN_FAIL=5,
    PGFM_CHARLIST_READY=6, PGFM_CHAR_ENTERED=7,
};

struct FrameWorkMsgData { FrameWorkMsg eMsg; int32 iParam0; int32 iParam1; };

class ShineFrameWork {
public:
    ShineFrameWork() : m_bRunning(false) {}
    virtual ~ShineFrameWork() {}
    virtual void OnStart()  {}
    virtual void OnStop()   {}
    virtual bool Update(float fDt) { return true; }
    virtual void Render()   {}
    virtual void OnMsg(const FrameWorkMsgData& rMsg) {}
    bool IsRunning() const { return m_bRunning; }
protected:
    bool m_bRunning;
    friend class ShineFrameWorkMgr;
};

class ShineFrameWorkMgr {
public:
    static ShineFrameWorkMgr& Get();
    void Register(FrameWorkID eId, ShineFrameWork* pkFW);
    void Start(FrameWorkID eId);
    void Stop();
    bool IsInRun(FrameWorkID eId) const;
    ShineFrameWork* GetFrameWork(FrameWorkID eId);
    void AddMsg(FrameWorkMsg eMsg, int32 iParam0=0, int32 iParam1=0);
    bool Update(float fDt);
    void Render();
    FrameWorkID GetActiveID() const { return m_eActiveId; }
private:
    ShineFrameWorkMgr() : m_eActiveId((FrameWorkID)-1), m_pkActive(NULL) {}
    std::map<FrameWorkID, ShineFrameWork*> m_kFrameWorks;
    FrameWorkID     m_eActiveId;
    ShineFrameWork* m_pkActive;
};
#define Pgg_kFrameMgr shine::ShineFrameWorkMgr::Get()
} // namespace shine
#endif
