// Server/DataReader/Tables.cpp
// Per-table DataProcess() row decoders. These are intentionally tolerant --
// missing columns fall back to defaults; rows are still indexed by InxName so
// later passes can deepen decoding without changing the call sites.
#include "Tables.h"
#include <stdlib.h>

namespace fiesta {

DEFINE_TAB(ItemInfoTab)
DEFINE_TAB(AbStateInfoTab)
DEFINE_TAB(ActSkillInfoTab)
DEFINE_TAB(KQTeamTable)
DEFINE_TAB(ChargedEffectTable)

static uint32 ColU32(const std::vector<std::string>& r, size_t i) {
    return (i < r.size() && !r[i].empty()) ? (uint32)strtoul(r[i].c_str(), NULL, 10) : 0u;
}
static uint16 ColU16(const std::vector<std::string>& r, size_t i) { return (uint16)ColU32(r, i); }
static int32  ColI32(const std::vector<std::string>& r, size_t i) {
    return (i < r.size() && !r[i].empty()) ? (int32)atoi(r[i].c_str()) : 0;
}

template <> void ITableBase<ItemInfoRec>::DataProcess(const std::vector<std::string>& r) {
    ItemInfoRec rec;
    rec.InxName    = ColU32(r, 0);
    rec.ItemNum    = ColU32(r, 1);
    rec.Type       = ColU16(r, 2);
    rec.SubType    = ColU16(r, 3);
    rec.LevelLimit = ColU16(r, 4);
    rec.ClassLimit = ColU16(r, 5);
    rec.Endure     = ColU16(r, 6);
    Push(rec.InxName, rec);
}

template <> void ITableBase<AbnormalStateInfoRec>::DataProcess(const std::vector<std::string>& r) {
    AbnormalStateInfoRec rec;
    rec.InxName      = ColU32(r, 0);
    rec.SaveType     = ColU16(r, 1);
    rec.ShelterFlags = ColU16(r, 2);
    rec.Duration     = ColI32(r, 3);
    Push(rec.InxName, rec);
}

template <> void ITableBase<ActiveSkillInfoRec>::DataProcess(const std::vector<std::string>& r) {
    ActiveSkillInfoRec rec;
    rec.InxName    = ColU32(r, 0);
    rec.SkillID    = ColU32(r, 1);
    rec.ClassLimit = ColU16(r, 2);
    rec.LevelLimit = ColU16(r, 3);
    rec.CoolDownMs = ColI32(r, 4);
    Push(rec.InxName, rec);
}

template <> void ITableBase<KQTeamRec>::DataProcess(const std::vector<std::string>& r) {
    KQTeamRec rec; rec.InxName = ColU32(r, 0); rec.TeamId = ColU16(r, 1); rec.MapId = ColU16(r, 2);
    Push(rec.InxName, rec);
}

template <> void ITableBase<ChargedItemEffectRec>::DataProcess(const std::vector<std::string>& r) {
    ChargedItemEffectRec rec;
    rec.InxName    = ColU32(r, 0);
    rec.EffectKind = ColU16(r, 1);
    rec.ParamA     = ColI32(r, 2);
    rec.ParamB     = ColI32(r, 3);
    Push(rec.InxName, rec);
}

} // namespace fiesta
