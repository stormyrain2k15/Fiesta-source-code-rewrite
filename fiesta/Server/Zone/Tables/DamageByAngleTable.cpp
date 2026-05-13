// Server/Zone/Tables/DamageByAngleTable.cpp
// FEATURE: world-creation -- World/DamageByAngle.txt binder.
// Per-attack-angle damage scalar (chr-attacker and mob-attacker
// curves). Damage bonus is stored x1000; lookup linearly interpolates
// between the sparse anchor rows the file ships with.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

DamageByAngleTable& DamageByAngleTable::Get() { static DamageByAngleTable s; return s; }

bool DamageByAngleTable::Load(const std::string& rRoot) {
    m_kChr.clear(); m_kMob.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\DamageByAngle.txt")) return false;
    // FEATURE: world-creation -- column read: DamagedAngle, DamageRate
    if (const TsTable* t = f.Find("DamageByAngle_Chr")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint16 a = (uint16)t->GetInt(r, "DamagedAngle");
            int32  s = (int32) t->GetInt(r, "DamageRate");
            m_kChr[a] = s;
        }
    }
    if (const TsTable* t = f.Find("DamageByAngle_Mob")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint16 a = (uint16)t->GetInt(r, "DamagedAngle");
            int32  s = (int32) t->GetInt(r, "DamageRate");
            m_kMob[a] = s;
        }
    }
    return true;
}

static int32 LookupAngle(const std::map<uint16, int32>& m, uint16 a) {
    if (m.empty()) return 1000;
    std::map<uint16, int32>::const_iterator it = m.lower_bound(a);
    if (it == m.end()) { --it; return it->second; }
    if (it == m.begin()) return it->second;
    int32 hi = it->second; uint16 hiA = it->first;
    --it;
    int32 lo = it->second; uint16 loA = it->first;
    if (hiA == loA) return lo;
    return lo + (int32)((int64)(hi - lo) * (int64)(a - loA) / (int64)(hiA - loA));
}

int32 DamageByAngleTable::ChrAttackerScalerX1k(uint16 a) const { return LookupAngle(m_kChr, a); }
int32 DamageByAngleTable::MobAttackerScalerX1k(uint16 a) const { return LookupAngle(m_kMob, a); }

} // namespace shine
