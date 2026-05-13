// Client/Framework/ShineFrameWork.cpp
#include "ShineFrameWork.h"
#include "../../Server/Shared/ShineLogSystem.h"

namespace shine {

ShineFrameWorkMgr& ShineFrameWorkMgr::Get() {
    static ShineFrameWorkMgr s; return s;
}

void ShineFrameWorkMgr::Register(FrameWorkID eId, ShineFrameWork* pkFW) {
    m_kFrameWorks[eId] = pkFW;
}

void ShineFrameWorkMgr::Start(FrameWorkID eId) {
    // Stop current
    if (m_pkActive) {
        m_pkActive->OnStop();
        m_pkActive->m_bRunning = false;
        SHINELOG_INFO("FrameWorkMgr: stopped %d", (int)m_eActiveId);
    }

    // Start new
    std::map<FrameWorkID, ShineFrameWork*>::iterator it = m_kFrameWorks.find(eId);
    if (it == m_kFrameWorks.end()) {
        SHINELOG_ERROR("FrameWorkMgr: unknown FrameWorkID %d", (int)eId);
        m_pkActive   = NULL;
        m_eActiveId  = (FrameWorkID)-1;
        return;
    }

    m_eActiveId = eId;
    m_pkActive  = it->second;
    m_pkActive->m_bRunning = true;
    m_pkActive->OnStart();
    SHINELOG_INFO("FrameWorkMgr: started %d", (int)eId);
}

void ShineFrameWorkMgr::Stop() {
    if (m_pkActive) {
        m_pkActive->OnStop();
        m_pkActive->m_bRunning = false;
        m_pkActive  = NULL;
        m_eActiveId = (FrameWorkID)-1;
    }
}

bool ShineFrameWorkMgr::IsInRun(FrameWorkID eId) const {
    return m_eActiveId == eId && m_pkActive && m_pkActive->m_bRunning;
}

ShineFrameWork* ShineFrameWorkMgr::GetFrameWork(FrameWorkID eId) {
    std::map<FrameWorkID, ShineFrameWork*>::iterator it = m_kFrameWorks.find(eId);
    return (it == m_kFrameWorks.end()) ? NULL : it->second;
}

void ShineFrameWorkMgr::AddMsg(FrameWorkMsg eMsg, int32 iParam0, int32 iParam1) {
    if (!m_pkActive) return;
    FrameWorkMsgData msg;
    msg.eMsg    = eMsg;
    msg.iParam0 = iParam0;
    msg.iParam1 = iParam1;
    m_pkActive->OnMsg(msg);
}

bool ShineFrameWorkMgr::Update(float fDt) {
    if (!m_pkActive) return true;
    return m_pkActive->Update(fDt);
}

void ShineFrameWorkMgr::Render() {
    if (m_pkActive) m_pkActive->Render();
}

} // namespace shine
