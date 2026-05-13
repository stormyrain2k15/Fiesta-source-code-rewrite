// Server/Zone/ShineObject.cpp
#include "ShineObject.h"
#include "FreeStatSystem.h"
#include <stdlib.h>

namespace shine {

ShineObject::ShineObject()
    : m_uiHandle(INVALID_HANDLE), m_uiMap(0),
      m_iHP(1), m_iMaxHP(1), m_iSP(0), m_iMaxSP(0) {}
ShineObject::~ShineObject() {}

ShinePlayer::ShinePlayer()
    : m_uiCharID(0), m_uiAcctID(0), m_uiLevel(1), m_uiClass(0),
      m_iAdminLevel(0),
      m_uiExp(0), m_uiFame(0), m_uiMoney(0),
      m_uiSTR(0), m_uiEND(0), m_uiINT(0), m_uiDEX(0), m_uiMEN(0),
      m_uiFreeStat(0), m_uiSkillPoint(0),
      m_pkSession(NULL) {
    m_iMaxHP = 100; m_iHP = 100; m_iMaxSP = 50; m_iSP = 50;
}

void ShinePlayer::AddMoney(int64 dMoney) {
    if (dMoney >= 0) {
        m_uiMoney += (uint64)dMoney;
    } else {
        uint64 u = (uint64)(-dMoney);
        m_uiMoney = (u > m_uiMoney) ? 0 : (m_uiMoney - u);
    }
}
void ShinePlayer::AddFame(int32 dFame) {
    int64 v = (int64)m_uiFame + dFame;
    if (v < 0) v = 0;
    m_uiFame = (uint32)v;
}

void ShinePlayer::BuildFreeStatLedger(FreeStatLedger& rOut) const {
    rOut.nSTR = (int32)m_uiSTR;
    rOut.nEND = (int32)m_uiEND;
    rOut.nDEX = (int32)m_uiDEX;
    rOut.nINT = (int32)m_uiINT;
    rOut.nMEN = (int32)m_uiMEN;
    rOut.nUnspentPoints = (int32)m_uiFreeStat;
}

void ShinePlayer::ApplyFreeStatLedger(const FreeStatLedger& rIn) {
    m_uiSTR      = (uint16)(rIn.nSTR < 0 ? 0 : rIn.nSTR);
    m_uiEND      = (uint16)(rIn.nEND < 0 ? 0 : rIn.nEND);
    m_uiDEX      = (uint16)(rIn.nDEX < 0 ? 0 : rIn.nDEX);
    m_uiINT      = (uint16)(rIn.nINT < 0 ? 0 : rIn.nINT);
    m_uiMEN      = (uint16)(rIn.nMEN < 0 ? 0 : rIn.nMEN);
    m_uiFreeStat = (uint16)(rIn.nUnspentPoints < 0 ? 0 : rIn.nUnspentPoints);
}

void ShinePlayer::FillFromCharLogin(AccountID a, CharID c, const std::string& n, uint16 lvl, uint16 cls) {
    m_uiAcctID = a; m_uiCharID = c; m_kName = n; m_uiLevel = lvl; m_uiClass = cls;
    m_kInv.SetOwner(c);
    m_iMaxHP = 80 + 20 * (int32)lvl;
    m_iMaxSP = 30 + 10 * (int32)lvl;
    m_iHP = m_iMaxHP; m_iSP = m_iMaxSP;
}

namespace {
    inline uint16 SafeU16(const DBRecord& r, size_t i) {
        const std::string& s = r.Get(i);
        return s.empty() ? (uint16)0 : (uint16)atoi(s.c_str());
    }
    inline uint32 SafeU32(const DBRecord& r, size_t i) {
        const std::string& s = r.Get(i);
        return s.empty() ? 0u : (uint32)strtoul(s.c_str(), NULL, 10);
    }
    inline uint64 SafeU64(const DBRecord& r, size_t i) {
        const std::string& s = r.Get(i);
        return s.empty() ? (uint64)0 : (uint64)_strtoui64(s.c_str(), NULL, 10);
    }
    inline int32 SafeI32(const DBRecord& r, size_t i) {
        const std::string& s = r.Get(i);
        return s.empty() ? 0 : atoi(s.c_str());
    }
}

void ShinePlayer::LoadFromCharDBRow(const DBRecord& r) {
    m_uiCharID     = SafeU32(r, 0);
    m_kName        = r.Get(1);
    m_uiAcctID     = SafeU32(r, 2);
    m_iAdminLevel  = SafeI32(r, 3);
    m_uiLevel      = SafeU16(r, 4); if (m_uiLevel == 0) m_uiLevel = 1;
    m_uiExp        = SafeU64(r, 5);
    m_uiFame       = SafeU32(r, 6);
    m_uiMoney      = SafeU64(r, 7);
    m_kLoginZone   = r.Get(8);                 // destination map name
    m_uiClass      = SafeU16(r, 9);
    // Mirror the character-id into the inventory so every Add/Remove
    // routes a Item_Create / Item_Delete proc through the CharDB exe.
    m_kInv.SetOwner(m_uiCharID);
    m_uiSTR        = SafeU16(r, 10);
    m_uiEND        = SafeU16(r, 11);
    m_uiINT        = SafeU16(r, 12);
    m_uiDEX        = SafeU16(r, 13);
    m_uiMEN        = SafeU16(r, 14);
    m_uiFreeStat   = SafeU16(r, 15);
    m_uiSkillPoint = SafeU16(r, 16);

    // Recompute caps. ParamN.txt rows handle the per-class detail; this
    // fallback keeps the player on its feet if the table isn't loaded yet.
    m_iMaxHP = 80 + 20 * (int32)m_uiLevel + (int32)m_uiEND * 14;
    m_iMaxSP = 30 + 10 * (int32)m_uiLevel + (int32)m_uiINT * 12;
    m_iHP = m_iMaxHP; m_iSP = m_iMaxSP;
}

} // namespace shine
