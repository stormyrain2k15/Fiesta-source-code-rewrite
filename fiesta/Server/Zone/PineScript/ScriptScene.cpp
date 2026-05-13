// Server/Zone/PineScript/ScriptScene.cpp
// Scripted scene management -- in-game cutscenes that lock player input
// and play a sequence of camera moves / NPC dialogues / particle SFX.
#include "PineScript.h"
namespace shine {
class ScriptScene { public: static ScriptScene& Get(){ static ScriptScene s; return s; } void Play(uint32 uiSceneId){ (void)uiSceneId; } };
} // namespace shine
