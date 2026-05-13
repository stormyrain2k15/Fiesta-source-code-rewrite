// Server/Zone/Tables/MiniHouseTables.cpp
// FEATURE: world-creation -- moved from GroupTables.cpp under
// the one-cpp-per-shn convention (docs/PER_SHN_CONVENTION.md).
#include "BindMacros.h"
#include "../GroupTables.h"

namespace shine {

MiniHouseTables& MiniHouseTables::Get() { static MiniHouseTables s; return s; }
void MiniHouseTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouse")) {
        ITER_ROWS(t) {
            LegacyMiniHouseRow rec;
            rec.uiID            = ShnGetU32(*t, _r, "ID");
            rec.kInxName        = ShnGetStr(*t, _r, "InxName");
            rec.kName           = ShnGetStr(*t, _r, "Name");
            rec.uiGrade         = ShnGetU32(*t, _r, "Grade");
            rec.uiDurationHour  = ShnGetU32(*t, _r, "DurationHour");
            m_kHouseById[rec.uiID] = m_kHouse.size();
            m_kHouse.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouseFurniture")) {
        ITER_ROWS(t) {
            MiniHouseFurnRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kInxName   = ShnGetStr(*t, _r, "InxName");
            rec.uiCategory = ShnGetU32(*t, _r, "Category");
            rec.uiHP       = ShnGetU32(*t, _r, "HP");
            m_kFurnById[rec.uiID] = m_kFurn.size();
            m_kFurn.push_back(rec);
        }
    }
}
const LegacyMiniHouseRow*     MiniHouseTables::Find    (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kHouseById.find(uiID);
    return (it == m_kHouseById.end()) ? NULL : &m_kHouse[it->second]; }
const MiniHouseFurnRow* MiniHouseTables::FindFurn(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kFurnById.find(uiID);
    return (it == m_kFurnById.end()) ? NULL : &m_kFurn[it->second]; }

GuildTables2& GuildTables2::Get() { static GuildTables2 s; return s; }
void GuildTables2::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildAcademy")) {
        ITER_ROWS(t) {
            LegacyGuildAcademyRow rec;
            rec.kBuffName        = ShnGetStr(*t, _r, "BuffName");
            rec.uiLeastJoinTime  = ShnGetU32(*t, _r, "LeastJoinTime");
            rec.uiRankAggregation= ShnGetU32(*t, _r, "RankAggregationTime");
            m_kAcademy.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildGradeData")) {
        ITER_ROWS(t) {
            LegacyGuildGradeDataRow rec;
            rec.uiGrade       = ShnGetU32(*t, _r, "Grade");
            rec.uiMaxMember   = ShnGetU32(*t, _r, "MaxMember");
            rec.uiStorageSlot = ShnGetU32(*t, _r, "StorageSlot");
            m_kGradeById[rec.uiGrade] = m_kGrade.size();
            m_kGrade.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournament")) {
        ITER_ROWS(t) {
            GuildTournamentRow rec;
            rec.uiGTNo        = ShnGetU32(*t, _r, "GTNo");
            rec.uiMatchNumber = ShnGetU32(*t, _r, "MatchNumber");
            rec.uiPrizeCoin   = ShnGetU32(*t, _r, "PrizeCoin");
            m_kGTById[rec.uiGTNo] = m_kGT.size();
            m_kGT.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentReward")) {
        ITER_ROWS(t) {
            LegacyGuildTournamentRewardRow rec;
            rec.uiRank = ShnGetU32(*t, _r, "Rank");
            rec.uiCoin = ShnGetU32(*t, _r, "Coin");
            rec.uiFame = ShnGetU32(*t, _r, "Fame");
            m_kRewardByRank[rec.uiRank] = m_kReward.size();
            m_kReward.push_back(rec);
        }
    }
}
const LegacyGuildGradeDataRow*        GuildTables2::FindGrade (uint32 uiG) const {
    std::map<uint32, size_t>::const_iterator it = m_kGradeById.find(uiG);
    return (it == m_kGradeById.end()) ? NULL : &m_kGrade[it->second]; }
const GuildTournamentRow*       GuildTables2::FindGT    (uint32 uiN) const {
    std::map<uint32, size_t>::const_iterator it = m_kGTById.find(uiN);
    return (it == m_kGTById.end()) ? NULL : &m_kGT[it->second]; }
const LegacyGuildTournamentRewardRow* GuildTables2::FindReward(uint32 uiR) const {
    std::map<uint32, size_t>::const_iterator it = m_kRewardByRank.find(uiR);
    return (it == m_kRewardByRank.end()) ? NULL : &m_kReward[it->second]; }


} // namespace shine
