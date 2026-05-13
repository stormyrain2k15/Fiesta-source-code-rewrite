// Server/DataReader/SHN/CharacterTitleData.cpp
#include "CharacterTitleData.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

CharacterTitleDataShn& CharacterTitleDataShn::Get() {
    static CharacterTitleDataShn s; return s;
}

void CharacterTitleDataShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("CharacterTitleData");
    if (!t) { SHINELOG_WARN("CharacterTitleData.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        CharacterTitleDataRow rec;
        rec.uiType    = ShnGetU32(*t, _r, "Type");
        rec.uiPermit  = ShnGetU32(*t, _r, "Permit");
        rec.uiRefresh = ShnGetU32(*t, _r, "Refresh");
        rec.kTitle0   = ShnGetStr(*t, _r, "Title0");
        rec.uiValue0  = ShnGetU32(*t, _r, "Value0");
        rec.uiFame0   = ShnGetU32(*t, _r, "Fame0");
        rec.kTitle1   = ShnGetStr(*t, _r, "Title1");
        rec.uiValue1  = ShnGetU32(*t, _r, "Value1");
        rec.uiFame1   = ShnGetU32(*t, _r, "Fame1");
        rec.kTitle2   = ShnGetStr(*t, _r, "Title2");
        rec.uiValue2  = ShnGetU32(*t, _r, "Value2");
        rec.uiFame2   = ShnGetU32(*t, _r, "Fame2");
        rec.kTitle3   = ShnGetStr(*t, _r, "Title3");
        rec.uiValue3  = ShnGetU32(*t, _r, "Value3");
        rec.uiFame3   = ShnGetU32(*t, _r, "Fame3");
        m_kByType[rec.uiType] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("CharacterTitleData.shn: %u rows", (uint32)m_kRows.size());
}

const CharacterTitleDataRow* CharacterTitleDataShn::FindByType(uint32 uiType) const {
    std::map<uint32, size_t>::const_iterator it = m_kByType.find(uiType);
    return (it == m_kByType.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
