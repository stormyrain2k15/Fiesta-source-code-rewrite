// Server/Zone/GroupTables.cpp
// Each Bind() walks the corresponding ShnFile's rows once and builds a
// typed vector + index map. Column reads are by NAME (`ShnGetU32(t, i,
// "ColName")`) so a future drop that renames or reorders columns lights up
// the missing-column path (zero-fill / empty-string) instead of silently
// reading the wrong field. The trade-off vs index reads is one O(N) name
// scan per accessor binding -- negligible at boot.
#include "GroupTables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

#define BIND_BEGIN(VAR, NAME) \
    const ShnFile* VAR = ShnRegistry::Get().GetTable(NAME); \
    if (!VAR) { SHINELOG_WARN("%s.shn missing -- group accessor disabled", NAME); return; }

#define ITER_ROWS(VAR) for (size_t _r = 0; _r < (VAR)->Rows().size(); ++_r)

// =============================================================================
//  ItemTables / MobTables / SkillTables / MapTables / AbStateTables
//  -- moved to /Tables/<Name>.cpp under the one-cpp-per-shn convention
//  adopted 2026-02. See docs/SCHEMA_COVERAGE.md for the policy and
//  Build/CI/audit_unwired_loads.py for the canary that catches
//  regressions where a Bind() exists but is never called from
//  BindAllGroupTables() below.
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

PupTables& PupTables::Get() { static PupTables s; return s; }
void PupTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupMain")) {
        ITER_ROWS(t) {
            PupMainRow rec;
            rec.uiPupID    = ShnGetU32(*t, _r, "PupID");
            rec.kPupIDX    = ShnGetStr(*t, _r, "PupIDX");
            rec.kItemIDX   = ShnGetStr(*t, _r, "ItemIDX");
            rec.uiPupSpeed = ShnGetU32(*t, _r, "PupSpeed");
            m_kMainById[rec.uiPupID] = m_kMain.size();
            m_kMain.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupServer")) {
        ITER_ROWS(t) {
            PupServerRow rec;
            rec.uiPupID    = ShnGetU32(*t, _r, "PupID");
            rec.uiSpawnHP  = ShnGetU32(*t, _r, "SpawnHP");
            rec.uiSpawnSP  = ShnGetU32(*t, _r, "SpawnSP");
            rec.uiCooldown = ShnGetU32(*t, _r, "Cooldown");
            rec.uiAggro    = ShnGetU32(*t, _r, "Aggro");
            m_kServerById[rec.uiPupID] = m_kServer.size();
            m_kServer.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupCase")) {
        ITER_ROWS(t) {
            PupCaseRow rec;
            rec.uiCaseID   = ShnGetU32(*t, _r, "CaseID");
            rec.uiPriority = ShnGetU32(*t, _r, "Priority");
            rec.uiAction   = ShnGetU32(*t, _r, "Action");
            m_kCaseById[rec.uiCaseID] = m_kCase.size();
            m_kCase.push_back(rec);
        }
    }
}
const PupMainRow*   PupTables::FindMain  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMainById.find(uiID);
    return (it == m_kMainById.end()) ? NULL : &m_kMain[it->second]; }
const PupServerRow* PupTables::FindServer(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kServerById.find(uiID);
    return (it == m_kServerById.end()) ? NULL : &m_kServer[it->second]; }
const PupCaseRow*   PupTables::FindCase  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kCaseById.find(uiID);
    return (it == m_kCaseById.end()) ? NULL : &m_kCase[it->second]; }

MountTables& MountTables::Get() { static MountTables s; return s; }
void MountTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MoverMain")) {
        ITER_ROWS(t) {
            MoverMainRow rec;
            rec.uiMoverID = ShnGetU32(*t, _r, "MoverID");
            rec.kMoverIDX = ShnGetStr(*t, _r, "MoverIDX");
            rec.uiCastMs  = ShnGetU32(*t, _r, "CastingTime");
            rec.uiCoolMs  = ShnGetU32(*t, _r, "CoolTime");
            rec.uiRun     = ShnGetU32(*t, _r, "RunSpeed");
            rec.uiWalk    = ShnGetU32(*t, _r, "WalkSpeed");
            rec.uiHours   = ShnGetU32(*t, _r, "DurationHour");
            rec.uiMaxSlot = ShnGetU32(*t, _r, "MaxCharSlot");
            m_kMainById[rec.uiMoverID] = m_kMain.size();
            m_kMain.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MoverItem")) {
        ITER_ROWS(t) {
            MoverItemRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kItemIDX = ShnGetStr(*t, _r, "ItemIDX");
            m_kItemById[rec.uiID] = m_kItem.size();
            m_kItem.push_back(rec);
        }
    }
}
const MoverMainRow* MountTables::FindMain(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMainById.find(uiID);
    return (it == m_kMainById.end()) ? NULL : &m_kMain[it->second]; }
const MoverItemRow* MountTables::FindItem(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kItemById.find(uiID);
    return (it == m_kItemById.end()) ? NULL : &m_kItem[it->second]; }

MiniHouseTables& MiniHouseTables::Get() { static MiniHouseTables s; return s; }
void MiniHouseTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouse")) {
        ITER_ROWS(t) {
            MiniHouseRow rec;
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
const MiniHouseRow*     MiniHouseTables::Find    (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kHouseById.find(uiID);
    return (it == m_kHouseById.end()) ? NULL : &m_kHouse[it->second]; }
const MiniHouseFurnRow* MiniHouseTables::FindFurn(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kFurnById.find(uiID);
    return (it == m_kFurnById.end()) ? NULL : &m_kFurn[it->second]; }

GuildTables2& GuildTables2::Get() { static GuildTables2 s; return s; }
void GuildTables2::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildAcademy")) {
        ITER_ROWS(t) {
            GuildAcademyRow rec;
            rec.kBuffName        = ShnGetStr(*t, _r, "BuffName");
            rec.uiLeastJoinTime  = ShnGetU32(*t, _r, "LeastJoinTime");
            rec.uiRankAggregation= ShnGetU32(*t, _r, "RankAggregationTime");
            m_kAcademy.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildGradeData")) {
        ITER_ROWS(t) {
            GuildGradeDataRow rec;
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
            GuildTournamentRewardRow rec;
            rec.uiRank = ShnGetU32(*t, _r, "Rank");
            rec.uiCoin = ShnGetU32(*t, _r, "Coin");
            rec.uiFame = ShnGetU32(*t, _r, "Fame");
            m_kRewardByRank[rec.uiRank] = m_kReward.size();
            m_kReward.push_back(rec);
        }
    }
}
const GuildGradeDataRow*        GuildTables2::FindGrade (uint32 uiG) const {
    std::map<uint32, size_t>::const_iterator it = m_kGradeById.find(uiG);
    return (it == m_kGradeById.end()) ? NULL : &m_kGrade[it->second]; }
const GuildTournamentRow*       GuildTables2::FindGT    (uint32 uiN) const {
    std::map<uint32, size_t>::const_iterator it = m_kGTById.find(uiN);
    return (it == m_kGTById.end()) ? NULL : &m_kGT[it->second]; }
const GuildTournamentRewardRow* GuildTables2::FindReward(uint32 uiR) const {
    std::map<uint32, size_t>::const_iterator it = m_kRewardByRank.find(uiR);
    return (it == m_kRewardByRank.end()) ? NULL : &m_kReward[it->second]; }

CollectTables& CollectTables::Get() { static CollectTables s; return s; }
void CollectTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("CollectCard")) {
        ITER_ROWS(t) {
            CollectCardRow rec;
            rec.uiCardID        = ShnGetU32(*t, _r, "CC_CardID");
            rec.kItemInx        = ShnGetStr(*t, _r, "CC_ItemInx");
            rec.uiCardGradeType = ShnGetU32(*t, _r, "CC_CardGradeType");
            rec.uiMobGroup      = ShnGetU32(*t, _r, "CC_CardMobGroup");
            m_kCardById[rec.uiCardID] = m_kCards.size();
            m_kCards.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardReward")) {
        ITER_ROWS(t) {
            CollectCardRewardRow rec;
            rec.uiID      = ShnGetU32(*t, _r, "ID");
            rec.uiPercent = ShnGetU32(*t, _r, "Percent");
            rec.kReward   = ShnGetStr(*t, _r, "Reward");
            rec.uiQty     = ShnGetU32(*t, _r, "Qty");
            rec.uiBonus   = ShnGetU32(*t, _r, "Bonus");
            m_kRewardById[rec.uiID] = m_kReward.size();
            m_kReward.push_back(rec);
        }
    }
}
const CollectCardRow*       CollectTables::FindCard  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kCardById.find(uiID);
    return (it == m_kCardById.end()) ? NULL : &m_kCards[it->second]; }
const CollectCardRewardRow* CollectTables::FindReward(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kRewardById.find(uiID);
    return (it == m_kRewardById.end()) ? NULL : &m_kReward[it->second]; }

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
            SetItemRow rec;
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
const SetItemRow*         GradeRandomTables::FindSet  (uint32 uiI) const {
    std::map<uint32, size_t>::const_iterator it = m_kSetById.find(uiI);
    return (it == m_kSetById.end()) ? NULL : &m_kSet[it->second]; }

KQTables& KQTables::Get() { static KQTables s; return s; }
void KQTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuest")) {
        ITER_ROWS(t) {
            KingdomQuestRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kName      = ShnGetStr(*t, _r, "Name");
            rec.uiMinLevel = ShnGetU32(*t, _r, "MinLevel");
            rec.uiMaxLevel = ShnGetU32(*t, _r, "MaxLevel");
            rec.uiTeamSize = ShnGetU32(*t, _r, "TeamSize");
            m_kKQById[rec.uiID] = m_kKQ.size();
            m_kKQ.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuestRew")) {
        ITER_ROWS(t) {
            KingdomQuestRewRow rec;
            rec.uiID    = ShnGetU32(*t, _r, "ID");
            rec.uiRank  = ShnGetU32(*t, _r, "Rank");
            rec.kReward = ShnGetStr(*t, _r, "Reward");
            rec.uiQty   = ShnGetU32(*t, _r, "Qty");
            m_kRew.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuestMap")) {
        ITER_ROWS(t) {
            KingdomQuestMapRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kMapName = ShnGetStr(*t, _r, "MapName");
            rec.uiX      = ShnGetU32(*t, _r, "X");
            rec.uiY      = ShnGetU32(*t, _r, "Y");
            m_kMapById[rec.uiID] = m_kMap.size();
            m_kMap.push_back(rec);
        }
    }
}
const KingdomQuestRow*    KQTables::FindKQ (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kKQById.find(uiID);
    return (it == m_kKQById.end()) ? NULL : &m_kKQ[it->second]; }
const KingdomQuestRewRow* KQTables::FindRew(uint32 uiID, uint32 uiRank) const {
    for (size_t i = 0; i < m_kRew.size(); ++i)
        if (m_kRew[i].uiID == uiID && m_kRew[i].uiRank == uiRank)
            return &m_kRew[i];
    return NULL;
}
const KingdomQuestMapRow* KQTables::FindMap(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMapById.find(uiID);
    return (it == m_kMapById.end()) ? NULL : &m_kMap[it->second]; }

// =============================================================================
//  One-call binder
// =============================================================================
void BindAllGroupTables() {
    ItemTables::Get()        .Bind();
    MobTables::Get()         .Bind();
    SkillTables::Get()       .Bind();
    MapTables::Get()         .Bind();
    AbStateTables::Get()     .Bind();
    PresentationTables::Get().Bind();
    PupTables::Get()         .Bind();
    MountTables::Get()       .Bind();
    MiniHouseTables::Get()   .Bind();
    GuildTables2::Get()      .Bind();
    CollectTables::Get()     .Bind();
    GradeRandomTables::Get() .Bind();
    KQTables::Get()          .Bind();
    SHINELOG_INFO("BindAllGroupTables: done; ShnRegistry has %u tables",
                  (uint32)ShnRegistry::Get().size());
}

#undef BIND_BEGIN
#undef ITER_ROWS

} // namespace fiesta
