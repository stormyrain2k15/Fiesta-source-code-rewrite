// Server/Zone/Tables/GradeRandomTables.cpp
// FEATURE: world-creation -- moved from GroupTables.cpp under
// the one-cpp-per-shn convention (docs/PER_SHN_CONVENTION.md).
#include "BindMacros.h"
#include "../GroupTables.h"

namespace shine {

GradeRandomTables& GradeRandomTables::Get() { static GradeRandomTables s; return s; }
void GradeRandomTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GradeItemOption")) {
        ITER_ROWS(t) {
            GradeItemOptionRow rec;
            rec.uiItemIndex         = ShnGetU32(*t, _r, "ItemIndex");
            rec.uiSTR               = (uint16)ShnGetU32(*t, _r, "STR");
            rec.uiCON               = (uint16)ShnGetU32(*t, _r, "CON");
            rec.uiDEX               = (uint16)ShnGetU32(*t, _r, "DEX");
            rec.uiINT               = (uint16)ShnGetU32(*t, _r, "INT");
            rec.uiMEN               = (uint16)ShnGetU32(*t, _r, "MEN");
            rec.uiResistPoison      = (uint16)ShnGetU32(*t, _r, "ResistPoison");
            rec.uiResistDeaseas     = (uint16)ShnGetU32(*t, _r, "ResistDeaseas");
            rec.uiResistCurse       = (uint16)ShnGetU32(*t, _r, "ResistCurse");
            rec.uiResistMoveSpdDown = (uint16)ShnGetU32(*t, _r, "ResistMoveSpdDown");
            rec.uiCritical          = (uint16)ShnGetU32(*t, _r, "Critical");
            rec.uiToHitRate         = (uint16)ShnGetU32(*t, _r, "ToHitRate");
            rec.uiToHitPlus         = (uint16)ShnGetU32(*t, _r, "ToHitPlus");
            rec.uiToBlockRate       = (uint16)ShnGetU32(*t, _r, "ToBlockRate");
            rec.uiToBlockPlus       = (uint16)ShnGetU32(*t, _r, "ToBlockPlus");
            rec.uiMaxHP             = (uint16)ShnGetU32(*t, _r, "MaxHP");
            rec.uiMaxSP             = (uint16)ShnGetU32(*t, _r, "MaxSP");
            rec.uiMoveSpdRate       = (uint16)ShnGetU32(*t, _r, "MoveSpdRate");
            rec.uiAbsoluteAttack    = (uint16)ShnGetU32(*t, _r, "AbsoluteAttack");
            rec.uiPickupLimit       = (uint16)ShnGetU32(*t, _r, "PickupLimit");
            m_kGradeById[rec.uiItemIndex] = m_kGrade.size();
            m_kGrade.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("SetItem")) {
        ITER_ROWS(t) {
            LegacySetItemRow rec;
            rec.uiIndex  = ShnGetU32(*t, _r, "Index");
            rec.uiPiece  = ShnGetU32(*t, _r, "Piece");
            rec.uiEffect = ShnGetU32(*t, _r, "Effect");
            m_kSetById[rec.uiIndex] = m_kSet.size();
            m_kSet.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("RandomOption")) {
        ITER_ROWS(t) {
            RandomOptionRow2 rec;
            rec.kDropItemIndex    = ShnGetStr(*t, _r, "DropItemIndex");
            rec.uiRandomOptionType= ShnGetU32(*t, _r, "RandomOptionType");
            rec.iMin              = ShnGetI32(*t, _r, "Min");
            rec.iMax              = ShnGetI32(*t, _r, "Max");
            rec.uiTypeDropRate    = ShnGetU32(*t, _r, "TypeDropRate");
            m_kRandom.push_back(rec);
        }
    }
}
const GradeItemOptionRow* GradeRandomTables::FindGrade(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kGradeById.find(uiID);
    return (it == m_kGradeById.end()) ? NULL : &m_kGrade[it->second]; }
const LegacySetItemRow*         GradeRandomTables::FindSet  (uint32 uiI) const {
    std::map<uint32, size_t>::const_iterator it = m_kSetById.find(uiI);
    return (it == m_kSetById.end()) ? NULL : &m_kSet[it->second]; }


} // namespace shine
