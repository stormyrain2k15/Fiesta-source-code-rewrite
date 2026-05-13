// Server/Zone/SetItemData.h
#ifndef SHINE_ZONE_SETITEMDATA_H
#define SHINE_ZONE_SETITEMDATA_H

namespace shine {
class ShinePlayer;

class SetItemData {
public:
    static SetItemData& Get();
    bool Load();
    // Count how many pieces of the given set index the player has equipped.
    // Returns 0 until inventory iteration is wired .
    int  CountEquippedPieces(ShinePlayer* pkPlayer, const char* szSetIndex) const;
};

} // namespace shine
#endif
