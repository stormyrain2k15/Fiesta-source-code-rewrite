// Server/Zone/ShineObject.h
// world-object hierarchy. Players additionally own per-character
// gameplay state (skills, abnormal-state book) so the engine can dispatch
// without out-of-band lookups.
//                                  ShineNPC, ShinePet, ShineItemDrop, CharacterSkill,
//                                  AbnormalState
#ifndef FIESTA_ZONE_SHINEOBJECT_H
#define FIESTA_ZONE_SHINEOBJECT_H
#include "../Shared/ShineTypes.h"
#include "../DataServer/Common/Database.h"   // DBRecord
#include "AbState.h"
#include "SkillSystem.h"
#include "AggroList.h"
#include "Inventory.h"
#include "QuestSystem.h"
#include <string>

namespace fiesta {

class ClientSession;

struct Vec3 { float x,y,z; Vec3():x(0),y(0),z(0){} Vec3(float a,float b,float c):x(a),y(b),z(c){} };

enum ObjType { OT_PLAYER, OT_MOVER, OT_MOB, OT_NPC, OT_PET, OT_ITEMDROP };

class ShineObject {
public:
    ShineObject();
    virtual ~ShineObject();
    virtual ObjType GetType() const = 0;

    Handle  GetHandle() const { return m_uiHandle; }
    void    SetHandle(Handle h){ m_uiHandle = h; }

    MapID   GetMap()    const { return m_uiMap; }
    void    SetMap(MapID m)   { m_uiMap = m; }

    const Vec3& GetPos() const { return m_kPos; }
    void    SetPos(const Vec3& v) { m_kPos = v; }

    int32   GetHP() const { return m_iHP; }   void SetHP(int32 v) { m_iHP = v < 0 ? 0 : (v > m_iMaxHP ? m_iMaxHP : v); }
    int32   GetSP() const { return m_iSP; }   void SetSP(int32 v) { m_iSP = v < 0 ? 0 : (v > m_iMaxSP ? m_iMaxSP : v); }
    int32   GetMaxHP() const { return m_iMaxHP; }
    int32   GetMaxSP() const { return m_iMaxSP; }
    void    SetMaxHP(int32 v) { m_iMaxHP = v > 0 ? v : 1; if (m_iHP > m_iMaxHP) m_iHP = m_iMaxHP; }
    void    SetMaxSP(int32 v) { m_iMaxSP = v >= 0 ? v : 0; if (m_iSP > m_iMaxSP) m_iSP = m_iMaxSP; }
    bool    IsDead() const { return m_iHP <= 0; }

protected:
    Handle m_uiHandle;
    MapID  m_uiMap;
    Vec3   m_kPos;
    int32  m_iHP, m_iMaxHP;
    int32  m_iSP, m_iMaxSP;
};

class ShinePlayer : public ShineObject {
public:
    ShinePlayer();
    virtual ObjType GetType() const { return OT_PLAYER; }

    // -- identity -----------------------------------------------------------
    CharID         GetCharID() const { return m_uiCharID; }
    AccountID      GetAcctID() const { return m_uiAcctID; }
    const std::string& GetName() const { return m_kName; }
    uint16         GetLevel()  const { return m_uiLevel; }
    uint16         GetClass()  const { return m_uiClass; }
    int32          GetAdminLevel() const { return m_iAdminLevel; }
    const std::string& GetLoginZone() const { return m_kLoginZone; }
    uint32         GetMapID()      const { return m_uiMapID; }
    void           SetMapID(uint32 m)    { m_uiMapID = m; }

    // -- core stats ---------------------------------------------------------
    uint64         GetExp()    const { return m_uiExp; }
    uint32         GetFame()   const { return m_uiFame; }
    uint64         GetMoney()  const { return m_uiMoney; }
    uint16         GetSTR()    const { return m_uiSTR; }
    uint16         GetEND()    const { return m_uiEND; }
    uint16         GetINT()    const { return m_uiINT; }
    uint16         GetDEX()    const { return m_uiDEX; }
    uint16         GetMEN()    const { return m_uiMEN; }
    uint16         GetFreeStat() const { return m_uiFreeStat; }
    uint16         GetSkillPoint() const { return m_uiSkillPoint; }

    void           AddExp(uint64 dExp)   { m_uiExp   += dExp; }
    void           AddMoney(int64 dMoney);
    void           AddFame(int32 dFame);

    // -- session attachment -------------------------------------------------
    void           AttachSession(ClientSession* p) { m_pkSession = p; }
    ClientSession* GetSession() { return m_pkSession; }

    // -- subsystems owned by the player -------------------------------------
    CharacterSkill& Skills()  { return m_kSkills; }
    AbnormalState&  AbState() { return m_kAbState; }
    Inventory&      Inv()     { return m_kInv;    }
    CharQuest&      Quest()   { return m_kQuest;  }

    // -- DB-driven population -----------------------------------------------
    // Provisional fill used when the CharDB row hasn't arrived yet (legacy
    // call site signature kept).
    void   FillFromCharLogin(AccountID a, CharID c, const std::string& n, uint16 lvl, uint16 cls);

    // Real fill from a `p_Char_Login` result row. Column order mirrors the
    // documented stored procedure projection:
    //   0:nCharNo  1:sID  2:nUserNo  3:nAdminLevel  4:nLevel  5:nExp
    //   6:nFame    7:nMoney  8:sLoginZone  9:nClass
    //  10:nSTR  11:nEND  12:nINT  13:nDEX  14:nMEN
    //  15:nFreeStat  16:nSkillPoint
    // Missing tail columns are tolerated (filled from defaults).
    void   LoadFromCharDBRow(const DBRecord& rRow);

private:
    CharID         m_uiCharID;
    AccountID      m_uiAcctID;
    std::string    m_kName;
    std::string    m_kLoginZone;
    uint32         m_uiMapID;
    uint16         m_uiLevel;
    uint16         m_uiClass;
    int32          m_iAdminLevel;

    uint64         m_uiExp;
    uint32         m_uiFame;
    uint64         m_uiMoney;
    uint16         m_uiSTR, m_uiEND, m_uiINT, m_uiDEX, m_uiMEN;
    uint16         m_uiFreeStat;
    uint16         m_uiSkillPoint;

    ClientSession* m_pkSession;
    CharacterSkill m_kSkills;
    AbnormalState  m_kAbState;
    Inventory      m_kInv;
    CharQuest      m_kQuest;
};

class ShineMover : public ShineObject { public: virtual ObjType GetType() const { return OT_MOVER; } };
class ShineMob   : public ShineObject {
public:
    ShineMob() : m_uiSpecies(0), m_uiLevel(1), m_uiLastAttackMs(0) {}
    virtual ObjType GetType() const { return OT_MOB; }
    MobID      m_uiSpecies;
    uint16     m_uiLevel;       // copied from MobInfo.shn at spawn time
    AggroList  m_kAggro;
    uint64     m_uiLastAttackMs;
};
class ShineNPC   : public ShineObject { public: virtual ObjType GetType() const { return OT_NPC;   } uint32 m_uiNpcId; };
class ShinePet   : public ShineObject { public: virtual ObjType GetType() const { return OT_PET;   } CharID m_uiOwner; };
class ShineItemDrop: public ShineObject { public: virtual ObjType GetType() const { return OT_ITEMDROP; } ItemID m_uiItem; uint32 m_uiQty; };

} // namespace fiesta
#endif
