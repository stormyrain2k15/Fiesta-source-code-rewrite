// Server/Zone/CharClass.cpp
// Per-character class runtime. Class tree: Fig->{War,Gla,Kni,...}, etc.
// Sentinel=26, Savior=27, Joker=21. ClassName.shn rows index by ClassID.
#include "../Shared/ShineTypes.h"
namespace shine {
class CharClass {
public:
    static const char* Prefix    (uint8 cls);
    static uint8       BaseClassOf(uint8 cls);
};
const char* CharClass::Prefix(uint8 /*cls*/) { return ""; }
uint8 CharClass::BaseClassOf(uint8 cls)      { return cls; }
} // namespace shine
