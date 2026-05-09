// Server/Zone/AbnormalStateDictionary.h
// Name -> AbState id lookup. Built from AbState.shn at boot. Used by
// the &abstate admin command and by Lua bindings that take the abstate
// by string ("Stun", "Frozen", ...) instead of its numeric id.
#ifndef FIESTA_ZONE_ABNORMALSTATEDICTIONARY_H
#define FIESTA_ZONE_ABNORMALSTATEDICTIONARY_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>

namespace fiesta {

struct AbStateRow;

class AbnormalStateDictionary {
public:
    static AbnormalStateDictionary& Get();
    bool   Load();
    // Returns 0 if the name is not in the table (caller treats as
    // "no abstate" / no-op).
    uint32 Lookup(const std::string& rName) const;
    size_t Size() const { return m_kByName.size(); }

    // Direct access into the loaded AbStateTables row. NULL when the
    // id is unknown (the dictionary is the source of truth here, but
    // it goes through AbStateTables for the column data).
    const AbStateRow* GetRow(uint32 uiAb) const;

    // AbStateSaveType lookup. Returns the save-type id from the row
    // (used to gate which states persist across link/die/logoff via
    // AbStateSaveTypeInfo.shn).
    uint32 GetSaveType(uint32 uiAb) const;
private:
    AbnormalStateDictionary() {}
    std::map<std::string, uint32> m_kByName;
};

} // namespace fiesta
#endif
