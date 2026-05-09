// Server/Zone/ShineObject.cpp
#include "ShineObject.h"

namespace fiesta {

ShineObject::ShineObject() : m_uiHandle(INVALID_HANDLE), m_uiMap(0), m_iHP(1), m_iMaxHP(1), m_iSP(0), m_iMaxSP(0) {}
ShineObject::~ShineObject() {}

ShinePlayer::ShinePlayer()
    : m_uiCharID(0), m_uiAcctID(0), m_uiLevel(1), m_uiClass(0), m_pkSession(NULL) {
    m_iMaxHP = 100; m_iHP = 100; m_iMaxSP = 50; m_iSP = 50;
}

void ShinePlayer::FillFromCharLogin(AccountID a, CharID c, const std::string& n, uint16 lvl, uint16 cls) {
    m_uiAcctID = a; m_uiCharID = c; m_kName = n; m_uiLevel = lvl; m_uiClass = cls;
    // Provisional base stat scaling -- pinned to ParamN.txt rows when DataReader populates them.
    m_iMaxHP = 80 + 20 * (int32)lvl;
    m_iMaxSP = 30 + 10 * (int32)lvl;
    m_iHP = m_iMaxHP; m_iSP = m_iMaxSP;
}

} // namespace fiesta
