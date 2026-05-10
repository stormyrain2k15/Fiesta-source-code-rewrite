// Server/DataReader/SHN/SetItemEffect.h
// Auto-generated: one-file-per-SHN split for SetItemEffect.shn
#ifndef FIESTA_DATAREADER_SHN_SETITEMEFFECT_H
#define FIESTA_DATAREADER_SHN_SETITEMEFFECT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct SetItemEffectRow {
    std::string      kEffect;
    std::string      kDesc;
    uint8            uiUseSubject;
    std::string      kSkillGroup;
    std::string      kFrom;
    std::string      kTo;
    uint32           uiIndex;
    uint32           uiArgument;
};

class SetItemEffectShn {
public:
    static SetItemEffectShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<SetItemEffectRow>& Rows() const { return m_kRows; }
private:
    std::vector<SetItemEffectRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_SETITEMEFFECT_H
