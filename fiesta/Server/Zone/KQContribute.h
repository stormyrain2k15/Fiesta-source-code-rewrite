// Server/Zone/KQContribute.h
#ifndef SHINE_ZONE_KQCONTRIBUTE_H
#define SHINE_ZONE_KQCONTRIBUTE_H
#include "../Shared/ShineTypes.h"
#include <map>
namespace shine {
class KQContribute {
public:
    static KQContribute& Get();
    void   Add(uint32 uiKQID, uint32 cid, uint32 uiPoints);
    uint32 GetPoints(uint32 uiKQID, uint32 cid) const;
    void   Clear(uint32 uiKQID);
private:
    std::map<uint64, uint32> m_kPer;
};
} // namespace shine
#endif
