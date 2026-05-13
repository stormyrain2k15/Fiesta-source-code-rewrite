// Server/Zone/CharacterTitle/CharacterTitle.cpp
// "Titles" -- per-character earned banners (FirstKill, Boss-XYZ, KQ-Hero).
// Title definitions are world-global; per-character earned set is in CharDB.
#include "../../Shared/ShineTypes.h"
namespace shine {
class CharacterTitle {
public:
    static CharacterTitle& Get() { static CharacterTitle s; return s; }
    bool LoadShn() { return true; }
    bool Award(uint32 /*cid*/, uint32 /*uiTitleId*/) { return true; }
};
} // namespace shine
