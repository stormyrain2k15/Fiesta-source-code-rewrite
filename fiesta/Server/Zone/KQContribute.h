// Server/Zone/KQContribute.h
// WIRE-13 (Lyra, May 2026)
#ifndef FIESTA_ZONE_KQCONTRIBUTE_H
#define FIESTA_ZONE_KQCONTRIBUTE_H
#include "../Shared/ShineTypes.h"
#include <map>
namespace fiesta {
class KQContribute {
public:
    static KQContribute& Get();
    void   Add(uint32 uiKQID, uint32 cid, uint32 uiPoints);
    uint32 GetPoints(uint32 uiKQID, uint32 cid) const;
    void   Clear(uint32 uiKQID);
private:
    std::map<uint64, uint32> m_kPer;
};
} // namespace fiesta
#endif
