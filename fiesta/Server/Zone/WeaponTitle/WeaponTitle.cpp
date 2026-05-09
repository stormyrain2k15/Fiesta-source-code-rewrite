// Server/Zone/WeaponTitle/WeaponTitle.cpp
// Per-weapon-instance title earned through use (kill counter -> tier).
#include "../../Shared/ShineTypes.h"
namespace fiesta {
class WeaponTitle {
public:
    static WeaponTitle& Get() { static WeaponTitle s; return s; }
    void OnKillWith(uint64 /*uiItemKey*/) {}
};
} // namespace fiesta
