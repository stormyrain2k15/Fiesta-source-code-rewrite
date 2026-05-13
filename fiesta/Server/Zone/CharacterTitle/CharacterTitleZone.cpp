// Server/Zone/CharacterTitle/CharacterTitleZone.cpp
// Zone-side title hooks: award on mob/boss kill, broadcast on title equip.
#include "../../Shared/ShineTypes.h"
namespace shine {
void CharacterTitleZone_OnMobKill(uint32 /*cid*/, uint32 /*uiNpcID*/) {}
void CharacterTitleZone_OnEquip  (uint32 /*cid*/, uint32 /*uiTitleId*/) {}
} // namespace shine
