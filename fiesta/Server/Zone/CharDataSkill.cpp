// Server/Zone/CharDataSkill.cpp
// Persistence helper for the per-char skill list. CharLogin pulls; logout/
// learn/forget pushes via CharDBClient.
#include "CharDBClient.h"
namespace fiesta {
class CharDataSkill { public: static void OnLogout(uint32 /*cid*/) {} };
} // namespace fiesta
