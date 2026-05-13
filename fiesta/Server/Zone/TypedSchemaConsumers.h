// Server/Zone/TypedSchemaConsumers.h
// Runtime layer for typed Schemas.h rows that drive game-world
// construction. Three things live here:
//   * StateFieldTable     -- the per-map "auto-effect" registry. When a
//                            player joins a Field whose MapName is listed,
//                            the bound AbState index-name is applied.
//   * ItemActionResolver  -- the (Condition, Effect) pair attached to an
//                            ItemAction row. Used by ShineItemUse to gate
//                            and run the actual stat mutation.
//   * SubAbStateRegistry  -- the per-AbState chained sub-states. Looked
//                            up by AbnormalState::Apply when a parent
//                            row points at a sub-state inx-name.
#ifndef SHINE_ZONE_TYPED_SCHEMA_CONSUMERS_H
#define SHINE_ZONE_TYPED_SCHEMA_CONSUMERS_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace shine {

class ShinePlayer;

class ItemActionResolver {
public:
    static bool ConditionFires(uint16 uiConditionID, ShinePlayer* pkSubject,
                               ShinePlayer* pkObject, int32 iDistance);
    static void EffectApply  (uint16 uiEffectID,
                              ShinePlayer* pkSelf, ShinePlayer* pkTarget);
};

class StateFieldTable {
public:
    static StateFieldTable& Get();
    void Bind();
    // Called on every player Field-enter. If the map has an auto-state row,
    // we apply the bound AbState inx-name to the player's AbnormalState.
    void OnPlayerEnter(ShinePlayer* pk, const std::string& rMapID) const;
private:
    StateFieldTable() {}
    struct Row { std::string kAbStateInx; uint32 uiStateSet; };
    std::map<std::string, Row> m_kByMap;
};

class SubAbStateRegistry {
public:
    static SubAbStateRegistry& Get();
    void Bind();
    struct Row {
        uint32      uiID;
        std::string kInxName;
        uint32      uiStrength;
        uint32      uiType;
        uint8       uiSubType;
        uint32      uiKeepTimeMs;
        uint32      aActionIndex[4];
        uint32      aActionArg  [4];
    };
    const Row* Find     (uint32 uiID)                   const;
    const Row* FindByInx(const std::string& rInxName)   const;
private:
    SubAbStateRegistry() {}
    std::vector<Row>                 m_kRows;
    std::map<uint32, size_t>         m_kById;
    std::map<std::string, size_t>    m_kByInx;
};

void BindTypedSchemaConsumers();

} // namespace shine
#endif
