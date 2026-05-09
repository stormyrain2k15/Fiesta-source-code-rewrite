// Server/Zone/AbState.h
// 14 -- AbState / SubAbState / dictionaries / shelter.
// EVIDENCE: PDB_CONFIRMED  symbol: AbnormalState, AbnormalStateDictionary, AbnormalStateShelter,
//                                  SubAbstatePriority, AbStateInst
#ifndef FIESTA_ZONE_ABSTATE_H
#define FIESTA_ZONE_ABSTATE_H
#include "ShineObject.h"
#include <map>
#include <vector>

namespace fiesta {

struct AbStateInst {
    uint32 uiAbInxName;
    uint64 uiExpireMs;
    uint16 uiStack;
};

class AbnormalState {
public:
    void Apply  (uint32 uiAb, int32 iDurMs, uint16 uiStack = 1);
    void Remove (uint32 uiAb);
    void Tick   (uint64 uiNowMs);
    bool Has    (uint32 uiAb) const;
private:
    std::vector<AbStateInst> m_kList;
};

class AbnormalStateDictionary {
public:
    static AbnormalStateDictionary& Get();
    uint16 GetSaveType(uint32 uiAb)     const;
    uint16 GetShelterFlags(uint32 uiAb) const;
};

class AbnormalStateShelter {
public:
    static bool IsShelteredFrom(uint32 uiAb);
};

class SubAbstatePriority {
public:
    static bool ShouldStack(uint32 uiNew, uint32 uiOld);
};

} // namespace fiesta
#endif
