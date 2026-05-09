// Server/Zone/SetItemData.h
// WIRE-03 (Lyra, May 2026)
#ifndef FIESTA_ZONE_SETITEMDATA_H
#define FIESTA_ZONE_SETITEMDATA_H

namespace fiesta {
class ShinePlayer;

class SetItemData {
public:
    static SetItemData& Get();
    bool Load();
    // Count how many pieces of the given set index the player has equipped.
    // Returns 0 until inventory iteration is wired (CODEX-05).
    int  CountEquippedPieces(ShinePlayer* pkPlayer, const char* szSetIndex) const;
};

} // namespace fiesta
#endif
