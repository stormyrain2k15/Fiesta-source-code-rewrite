// Server/Zone/Tables/PresentationTables.cpp
// FEATURE: world-creation -- moved from GroupTables.cpp under
// the one-cpp-per-shn convention (docs/PER_SHN_CONVENTION.md).
#include "BindMacros.h"
#include "../GroupTables.h"

namespace fiesta {

// =============================================================================

PresentationTables& PresentationTables::Get() { static PresentationTables s; return s; }
void PresentationTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("HairInfo")) {
        ITER_ROWS(t) {
            HairInfoRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kIndexName = ShnGetStr(*t, _r, "IndexName");
            rec.kName      = ShnGetStr(*t, _r, "HairName");
            rec.uiGrade    = ShnGetU32(*t, _r, "Grade");
            rec.uiFighter  = ShnGetU32(*t, _r, "fighter");
            rec.uiArcher   = ShnGetU32(*t, _r, "archer");
            rec.uiCleric   = ShnGetU32(*t, _r, "cleric");
            rec.uiMage     = ShnGetU32(*t, _r, "mage");
            m_kHairById[rec.uiID] = m_kHair.size();
            m_kHair.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("FaceInfo")) {
        ITER_ROWS(t) {
            FaceInfoRow rec;
            rec.uiID      = ShnGetU32(*t, _r, "ID");
            rec.kFaceName = ShnGetStr(*t, _r, "FaceName");
            rec.uiGrade   = ShnGetU32(*t, _r, "Grade");
            m_kFaceById[rec.uiID] = m_kFace.size();
            m_kFace.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("ClassName")) {
        ITER_ROWS(t) {
            ClassNameRow rec;
            rec.uiClassID   = ShnGetU32(*t, _r, "ClassID");
            rec.kPrefix     = ShnGetStr(*t, _r, "acPrefix");
            rec.kEngName    = ShnGetStr(*t, _r, "acEngName");
            rec.kLocalName  = ShnGetStr(*t, _r, "acLocalName");
            m_kClassById[rec.uiClassID] = m_kClass.size();
            m_kClass.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("CharacterTitleData")) {
        ITER_ROWS(t) {
            CharacterTitleRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kInxName = ShnGetStr(*t, _r, "InxName");
            rec.uiGrade  = ShnGetU32(*t, _r, "Grade");
            m_kCharTitleById[rec.uiID] = m_kCharTitle.size();
            m_kCharTitle.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("WeaponTitleData")) {
        ITER_ROWS(t) {
            WeaponTitleRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kInxName = ShnGetStr(*t, _r, "InxName");
            m_kWeapTitleById[rec.uiID] = m_kWeapTitle.size();
            m_kWeapTitle.push_back(rec);
        }
    }
}
const HairInfoRow*       PresentationTables::FindHair (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kHairById.find(uiID);
    return (it == m_kHairById.end()) ? NULL : &m_kHair[it->second]; }
const FaceInfoRow*       PresentationTables::FindFace (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kFaceById.find(uiID);
    return (it == m_kFaceById.end()) ? NULL : &m_kFace[it->second]; }
const ClassNameRow*      PresentationTables::FindClass(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kClassById.find(uiID);
    return (it == m_kClassById.end()) ? NULL : &m_kClass[it->second]; }
const CharacterTitleRow* PresentationTables::FindCharTitle(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kCharTitleById.find(uiID);
    return (it == m_kCharTitleById.end()) ? NULL : &m_kCharTitle[it->second]; }
const WeaponTitleRow*    PresentationTables::FindWeapTitle(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kWeapTitleById.find(uiID);
    return (it == m_kWeapTitleById.end()) ? NULL : &m_kWeapTitle[it->second]; }

} // namespace fiesta
