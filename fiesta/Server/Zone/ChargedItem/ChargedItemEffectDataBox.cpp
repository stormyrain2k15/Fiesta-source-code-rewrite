// Server/Zone/ChargedItem/ChargedItemEffectDataBox.cpp
// Per-row decoded ChargedItemEffect data. Mirrors the ChargedItemEffect
// SHN columns. Lookup is by buff id.
#include "../../DataReader/ShnRegistry.h"
#include <map>
namespace fiesta {
struct ChargedEffectRow { uint32 uiBuffId; int32 nATK, nDEF, nMaxHP, nMaxSP, nMoveSpeed; };
class ChargedItemEffectDataBox {
public:
    static ChargedItemEffectDataBox& Get() { static ChargedItemEffectDataBox s; return s; }
    const ChargedEffectRow* Find(uint32 uiBuffId) const {
        std::map<uint32, ChargedEffectRow>::const_iterator it = m_kRows.find(uiBuffId);
        return (it == m_kRows.end()) ? NULL : &it->second;
    }
private:
    std::map<uint32, ChargedEffectRow> m_kRows;
};
} // namespace fiesta
