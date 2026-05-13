// Server/Zone/GambleHouse/SlotMachine/SlotMachine.cpp
// "GB" slot machine. 3 reels, weighted symbol distribution sourced from
// GambleItemTable.shn on the WM. Spin payload returns one item id (0
// when no prize).
#include "../../../Shared/ShineLogSystem.h"
namespace shine {
class SlotMachine { public: static SlotMachine& Get(){ static SlotMachine s; return s; } };
} // namespace shine
