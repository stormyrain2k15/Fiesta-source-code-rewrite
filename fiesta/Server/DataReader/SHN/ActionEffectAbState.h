// Server/DataReader/SHN/ActionEffectAbState.h
// Auto-generated: one-file-per-SHN split for ActionEffectAbState.shn
#ifndef SHINE_DATAREADER_SHN_ACTIONEFFECTABSTATE_H
#define SHINE_DATAREADER_SHN_ACTIONEFFECTABSTATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ActionEffectAbStateRow {
    std::string      kInxName;
    uint32           uiStrength;
    uint16           uiItemActionID;
};

class ActionEffectAbStateShn {
public:
    static ActionEffectAbStateShn& Get();
    void Load();
    const ActionEffectAbStateRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ActionEffectAbStateRow>& Rows() const { return m_kRows; }
private:
    std::vector<ActionEffectAbStateRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ACTIONEFFECTABSTATE_H
