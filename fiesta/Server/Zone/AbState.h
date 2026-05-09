// Server/Zone/AbState.h
// AbState / SubAbState / dictionaries / shelter.
// Per-target buff/debuff ledger: holds active runtime rows resolved
// by AbStateRuntime, exposes apply/remove/tick + stat-mod queries.
#ifndef FIESTA_ZONE_ABSTATE_H
#define FIESTA_ZONE_ABSTATE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>

namespace fiesta {

class ShineObject;
struct AbStateRuntimeRow;     // see AbStateRuntime.h

class AbnormalState {
public:
    // Apply by AbState id (the dictionary lookup target). Returns true
    // when the effect actually fired -- false means the AbState row was
    // missing, the SubAbState link was empty, or the requested strength
    // tier had no row to clamp into.
    bool Apply  (uint32 uiAb, int32 iDurMs, uint16 uiStack = 1);
    // Apply at a specific Strength tier; used when the source skill
    // already knows the tier (e.g. SkillData.SubStrength).
    bool ApplyAt(uint32 uiAb, uint32 uiStrength, int32 iDurMs);
    // Apply a SubAbState row directly by InxName + Strength. This is
    // the path skill/item/map/mount effects use when the source data
    // points at a SubAbState InxName (e.g. SkillData.Sta1InxName ==
    // "SubStaSeverBone") rather than an AbState id. The runtime row
    // is created without an AbState parent (uiAbStateId == 0); the
    // dispatcher and stat-mod queries treat it identically.
    bool ApplySubByName(const char* szSubInxName, uint32 uiStrength,
                        int32 iDurMs);
    // Remove by AbState id; returns true if a row was actually removed.
    bool Remove (uint32 uiAb);
    // Remove every active row whose underlying SubAbState InxName matches.
    bool RemoveBySubInxName(const char* szSubInxName);
    // Drop every active row whose AbState DispelIndex matches the
    // requested category. Returns the number of rows dispelled.
    uint32 DispelByCategory(uint32 uiDispelIndex, bool bSubDispelMatch);
    void Tick   (ShineObject* pkOwner, uint64 uiNowMs);
    bool Has    (uint32 uiAb) const;
    bool HasInxName(const char* sz) const;
    size_t Count() const { return m_kRuntime.size(); }

    // Aggregated stat-modifier delta (x1000) from every active row that
    // touches the requested action id (kAbAction_StatMod*). Returned in
    // the same x1000 unit the rest of BattleStat uses; the caller adds
    // it to the post-equip stat at compose time.
    int32 StatModX1k(uint32 uiAction) const;

    // Mutable runtime row vector -- exposed to AbStateRuntime so the
    // shield / reflect gates can decrement absorb capacity in place.
    std::vector<AbStateRuntimeRow>& RuntimeRows() { return m_kRuntime; }
    const std::vector<AbStateRuntimeRow>& RuntimeRows() const { return m_kRuntime; }
private:
    std::vector<AbStateRuntimeRow> m_kRuntime;
};

class SubAbstatePriority {
public:
    // Apply-replace rule. Returns:
    //   true  -> caller should drop the existing instance and push the
    //            new one (same family, higher Strength, or AbState row's
    //            Duplicate flag set to 0 == "replace mode").
    //   false -> caller should keep the old instance untouched (e.g.
    //            the new one is weaker, or Duplicate=1 means stack and
    //            both rows live concurrently in the ledger).
    static bool ShouldReplace(uint32 uiNewAbId, uint32 uiOldAbId);
    static bool ShouldStack  (uint32 uiNewAbId, uint32 uiOldAbId);
};

} // namespace fiesta
#endif
