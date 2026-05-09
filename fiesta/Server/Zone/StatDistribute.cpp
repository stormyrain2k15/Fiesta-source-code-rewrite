// Server/Zone/StatDistribute.cpp
#include "StatDistribute.h"
#include "ClassParamTable.h"
#include "MoverTables.h"
#include "ShineObject.h"
#include "BattleTunables.h"

namespace fiesta {

void FillRawCharStatFromPlayer(RAWCHARSTAT& rOut, const ShinePlayer& kP) {
    rOut.Clear();
    rOut.nLevel = (int32)kP.GetLevel();
    rOut.bClass = (uint8)kP.GetClass();
    rOut.bJob   = 0;

    // Allocated free-stat portion -- BattleStat compose layers these on
    // top of the class baseline at the rates defined in BattleTunables.
    rOut.nSTR = (int32)kP.GetSTR();
    rOut.nEND = (int32)kP.GetEND();
    rOut.nDEX = (int32)kP.GetDEX();
    rOut.nINT = (int32)kP.GetINT();
    rOut.nMEN = (int32)kP.GetMEN();

    // Class progression baseline. Prefer ClassParamTable (per-class .txt
    // ladder); fall back to MoverMainTable (SHN-wide cap) so the engine
    // stays usable when only one of the two has loaded.
    const ClassParamRow* pCp =
        ClassParamTable::Get().Find((eShineClass)kP.GetClass(), kP.GetLevel());
    if (pCp) {
        rOut.nBaseHP        = pCp->nMaxHP;
        rOut.nBaseSP        = pCp->nMaxSP;
        rOut.nBaseATK       = pCp->nAtkPerAP * pCp->nStr / 100;   // AP * STR scaler
        rOut.nBaseMATK      = pCp->nAtkPerAP * pCp->nInt / 100;
        rOut.nBaseDEF       = pCp->nGrdStoneAC;
        rOut.nBaseMDEF      = pCp->nGrdStoneMR;
        rOut.nBaseMoveSpeed = 200;
        return;
    }
    MoverMainTable::Get().FillRaw(&rOut, (uint8)kP.GetClass(), kP.GetLevel());
}

void BuildBattleStat(BATTLESTAT* pOut,
                     const RAWCHARSTAT* pRaw,
                     const EQUIPSUMMARY* pEquip,
                     const BUFFMODIFIERS* pBuff) {
    if (!pOut || !pRaw) return;
    pOut->Clear();

    pOut->nLevel = pRaw->nLevel;
    pOut->bClass = pRaw->bClass;
    pOut->bJob   = pRaw->bJob;
    pOut->nSTR   = pRaw->nSTR;
    pOut->nEND   = pRaw->nEND;
    pOut->nDEX   = pRaw->nDEX;
    pOut->nINT   = pRaw->nINT;
    pOut->nMEN   = pRaw->nMEN;

    // Base stats from MoverMain (free-stat-derived fallback when 0).
    pOut->nMaxHP    = pRaw->nBaseHP    + pRaw->nEND * kHpPerEND;
    pOut->nMaxSP    = pRaw->nBaseSP    + pRaw->nINT * kSpPerINT;
    pOut->nATK      = pRaw->nBaseATK   + pRaw->nSTR * kAtkPerSTR;
    pOut->nMATK     = pRaw->nBaseMATK  + pRaw->nINT * kMatkPerINT;
    pOut->nDEF      = pRaw->nBaseDEF   + pRaw->nEND * kDefPerEND;
    pOut->nMDEF     = pRaw->nBaseMDEF;
    pOut->nMoveSpeed= pRaw->nBaseMoveSpeed;

    pOut->nAccuracy = pRaw->nDEX * kHitPerDEX;
    pOut->nCritical = pRaw->nDEX * kCritPerDEX;
    pOut->nDodge    = pRaw->nDEX * kDodgePerDEX;

    // Layer equipment.
    if (pEquip) {
        pOut->nATK    += pEquip->nATK + pEquip->nUpgradeATK;
        pOut->nMATK   += pEquip->nMATK;
        pOut->nRATK   += pEquip->nRATK;
        pOut->nDEF    += pEquip->nDEF + pEquip->nUpgradeDEF;
        pOut->nMDEF   += pEquip->nMDEF;
        pOut->nRDEF   += pEquip->nRDEF;
        pOut->nMaxHP  += pEquip->nHP;
        pOut->nMaxSP  += pEquip->nSP;
        pOut->nCritical+= pEquip->nCritical;
        pOut->nDodge  += pEquip->nDodge;
        pOut->nBlock  += pEquip->nBlock;
        pOut->nAccuracy+= pEquip->nAccuracy;
        pOut->nMoveSpeed+= pEquip->nMoveSpeed;
        for (int i = 0; i < ELEMENT_MAX; ++i) {
            pOut->nEleATK[i]    += pEquip->nEleATK[i];
            pOut->nEleResist[i] += pEquip->nEleResist[i];
        }
    }

    // Layer buffs / debuffs.
    if (pBuff) {
        pOut->nATK    += pBuff->nATK;
        pOut->nMATK   += pBuff->nMATK;
        pOut->nRATK   += pBuff->nRATK;
        pOut->nDEF    += pBuff->nDEF;
        pOut->nMDEF   += pBuff->nMDEF;
        pOut->nMaxHP  += pBuff->nMaxHP;
        pOut->nMaxSP  += pBuff->nMaxSP;
        pOut->nHPRegen+= pBuff->nHPRegen;
        pOut->nSPRegen+= pBuff->nSPRegen;
        pOut->nMoveSpeed+= pBuff->nMoveSpeed;
        pOut->nCritical+= pBuff->nCritical;
        pOut->nDodge  += pBuff->nDodge;
        pOut->nAccuracy+= pBuff->nAccuracy;
        pOut->nAbsorbDmg+= pBuff->nAbsorbDmg;
        pOut->nAbstateResist += pBuff->nAbstateResist;
        for (int i = 0; i < ELEMENT_MAX; ++i) {
            pOut->nEleATK[i]    += pBuff->nEleATK[i];
            pOut->nEleResist[i] += pBuff->nEleResist[i];
        }
    }

    // Clamp negatives to 0 -- a debuff cannot turn ATK below zero.
    if (pOut->nATK   < 0) pOut->nATK   = 0;
    if (pOut->nMATK  < 0) pOut->nMATK  = 0;
    if (pOut->nDEF   < 0) pOut->nDEF   = 0;
    if (pOut->nMDEF  < 0) pOut->nMDEF  = 0;
    if (pOut->nMaxHP < 1) pOut->nMaxHP = 1;
    if (pOut->nMaxSP < 0) pOut->nMaxSP = 0;
}

} // namespace fiesta

