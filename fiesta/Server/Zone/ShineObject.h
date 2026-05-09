// Server/Zone/ShineObject.h
// 06/08 -- entity base + ShinePlayer + ShineMover (mount/pet vehicle).
// EVIDENCE: PDB_CONFIRMED  symbol: ShineObject, ShinePlayer, ShineObjectClass::ShineMover
#ifndef FIESTA_ZONE_SHINEOBJECT_H
#define FIESTA_ZONE_SHINEOBJECT_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace fiesta {

class ClientSession;

enum ObjType { OT_NONE = 0, OT_PLAYER = 1, OT_MOB = 2, OT_NPC = 3, OT_ITEMDROP = 4, OT_PET = 5, OT_MOVER = 6 };

struct Vec3 { float x, y, z; Vec3() : x(0), y(0), z(0) {} Vec3(float a, float b, float c) : x(a), y(b), z(c) {} };

class ShineObject {
public:
    ShineObject();
    virtual ~ShineObject();
    virtual ObjType GetType() const { return OT_NONE; }

    Handle   GetHandle()  const { return m_uiHandle; }
    void     SetHandle(Handle h){ m_uiHandle = h; }
    MapID    GetMapId()   const { return m_uiMap; }
    const Vec3& GetPos() const  { return m_kPos; }
    void     SetPos(const Vec3& p) { m_kPos = p; }
    void     SetMap(MapID m)    { m_uiMap = m; }

    // Common stat
    int32    GetHP()      const { return m_iHP; }
    int32    GetMaxHP()   const { return m_iMaxHP; }
    int32    GetSP()      const { return m_iSP; }
    int32    GetMaxSP()   const { return m_iMaxSP; }
    void     SetHP(int32 v)     { m_iHP = v < 0 ? 0 : (v > m_iMaxHP ? m_iMaxHP : v); }
    void     SetSP(int32 v)     { m_iSP = v < 0 ? 0 : (v > m_iMaxSP ? m_iMaxSP : v); }

    bool     IsDead() const     { return m_iHP <= 0; }
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

    CharID         GetCharID() const { return m_uiCharID; }
    AccountID      GetAcctID() const { return m_uiAcctID; }
    const std::string& GetName() const { return m_kName; }
    uint16         GetLevel() const { return m_uiLevel; }
    uint16         GetClass() const { return m_uiClass; }

    void   AttachSession(ClientSession* p) { m_pkSession = p; }
    ClientSession* GetSession() { return m_pkSession; }

    void   FillFromCharLogin(AccountID a, CharID c, const std::string& n, uint16 lvl, uint16 cls);
private:
    CharID         m_uiCharID;
    AccountID      m_uiAcctID;
    std::string    m_kName;
    uint16         m_uiLevel;
    uint16         m_uiClass;
    ClientSession* m_pkSession;
};

class ShineMover : public ShineObject { public: virtual ObjType GetType() const { return OT_MOVER; } };
class ShineMob   : public ShineObject { public: virtual ObjType GetType() const { return OT_MOB;   } MobID m_uiSpecies; };
class ShineNPC   : public ShineObject { public: virtual ObjType GetType() const { return OT_NPC;   } uint32 m_uiNpcId; };
class ShinePet   : public ShineObject { public: virtual ObjType GetType() const { return OT_PET;   } CharID m_uiOwner; };
class ShineItemDrop: public ShineObject { public: virtual ObjType GetType() const { return OT_ITEMDROP; } ItemID m_uiItem; uint32 m_uiQty; };

} // namespace fiesta
#endif
