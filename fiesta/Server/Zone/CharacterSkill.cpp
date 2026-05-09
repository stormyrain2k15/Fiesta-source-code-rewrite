// Server/Zone/CharacterSkill.cpp
// Per-character skill list. Class declared in SkillSystem.h; this file
// owns the canonical filename. Ledger is kept anonymous to avoid ODR.
#include "SkillSystem.h"
#include "CharDBClient.h"
#include <map>
#include <set>
namespace fiesta { namespace {
struct CharacterSkillLedger {
    static CharacterSkillLedger& Get() { static CharacterSkillLedger s; return s; }
    std::map<uint32, std::set<uint32> > m_kPer;
};
}} // anonymous
