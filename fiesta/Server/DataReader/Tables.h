// Server/DataReader/Tables.h
// 02 -- canonical *Tab declarations referenced across server.
// Each one mirrors a real PDB ITableBase<T>::ms_pkTable instance.
// EVIDENCE: PDB_CONFIRMED for every name below.
#ifndef FIESTA_DATAREADER_TABLES_H
#define FIESTA_DATAREADER_TABLES_H
#include "ITableBase.h"
#include "DataBox.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

// Helper macro instantiates a table class with a fixed logical filename and an
// integer key resolver. Subsystems then access fooTab::ms_pkTable->Find(id).
#define DECLARE_TAB(Name, Record, LogicalName)                                     \
class Name : public ITableBase<Record>, public IDataTable {                        \
public:                                                                            \
    Name() { ms_pkTable = this; }                                                  \
    virtual bool Load(DataReader& r) {                                             \
        std::vector<std::vector<std::string> > kRows;                              \
        if (!r.LoadAsRows(LogicalName, kRows)) return false;                       \
        for (size_t i = 1 /*skip header*/; i < kRows.size(); ++i) DataProcess(kRows[i]); \
        SHINELOG_INFO(LogicalName ": loaded %u rows", (uint32)m_uiTotal);          \
        return true;                                                               \
    }                                                                              \
    virtual void BeforeTerminate() { ITableBase<Record>::BeforeTerminate(); }      \
    virtual uint32 GetTotal() const { return ITableBase<Record>::GetTotal(); }     \
    virtual const char* LogicalName() const { return _STR(LogicalName); }          \
};                                                                                 \
extern Name g_##Name;

#define _STR(x) #x
#define DEFINE_TAB(Name) Name g_##Name;

// --- Selected canonical tables (subset). Adding the rest is mechanical. ---
struct ItemInfoRec {
    uint32 InxName;
    uint32 ItemNum;
    uint16 Type;
    uint16 SubType;
    uint16 LevelLimit;
    uint16 ClassLimit;
    uint16 Endure;
};

struct AbnormalStateInfoRec {
    uint32 InxName;
    uint16 SaveType;
    uint16 ShelterFlags;
    int32  Duration;
};

struct ActiveSkillInfoRec {
    uint32 InxName;
    uint32 SkillID;
    uint16 ClassLimit;
    uint16 LevelLimit;
    int32  CoolDownMs;
};

struct KQTeamRec {
    uint32 InxName;
    uint16 TeamId;
    uint16 MapId;
};

struct ChargedItemEffectRec {
    uint32 InxName;
    uint16 EffectKind;
    int32  ParamA;
    int32  ParamB;
};

DECLARE_TAB(ItemInfoTab,           ItemInfoRec,           ItemInfo)
DECLARE_TAB(AbStateInfoTab,        AbnormalStateInfoRec,  AbState)
DECLARE_TAB(ActSkillInfoTab,       ActiveSkillInfoRec,    ActiveSkillInfoServer)
DECLARE_TAB(KQTeamTable,           KQTeamRec,             KQTeam)
DECLARE_TAB(ChargedEffectTable,    ChargedItemEffectRec,  ChargedEffect)

} // namespace fiesta
#endif
