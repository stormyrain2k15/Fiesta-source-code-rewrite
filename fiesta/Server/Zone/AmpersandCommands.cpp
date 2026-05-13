// Server/Zone/AmpersandCommands.cpp
#include "AmpersandCommands.h"
#include "LiveOpsBoosts.h"
#include "WMClient.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>
namespace shine {
// Default handler: log and succeed. Per-command bodies wire in pass 2.
static bool Cmd_default(ShinePlayer* pk, const std::vector<std::string>& a) {
    SHINELOG_INFO("&cmd by %s args=%u", pk?pk->GetName().c_str():"?", (uint32)a.size());
    return true;
}

// &luckyhour <minutes> [expX1k] [dropX1k]
//   minutes : duration window. 0 == open-ended (until &luckyhour stop).
//   expX1k  : EXP boost x1000 (ignored unless cross-zone path can't reach
//             WM; the canonical path uses GMEvent.shn-driven boosts).
//   dropX1k : Drop boost x1000 (same caveat).
// `&luckyhour stop` ends it server-wide.
//
// Wire path: Zone -> WM via NC_INTER_GMEVENT_TRIGGER_REQ -> WM
// re-broadcasts NC_INTER_BROADCAST_CMD kind=2 to every connected zone
// -> each zone calls GMEventManager_Zone::OnEventBroadcast which feeds
// LiveOpsBoosts. This guarantees every zone sees the same boost window.
//
// If the WM link is down (boot race / disconnect), we fall back to a
// local-only boost so the GM still gets visible feedback.
static bool Cmd_luckyhour(ShinePlayer* pk, const std::vector<std::string>& a) {
    bool bStop = (!a.empty() && (a[0] == "stop" || a[0] == "end" || a[0] == "off"));
    if (a.empty() || bStop) {
        if (!WMClient::Get().SendGMEventTrigger(kGMEvent_LuckyHour, 0, false)) {
            LiveOpsBoosts::Get().Stop();    // local fallback
            SHINELOG_WARN("&luckyhour STOP: WM offline, local-only");
        }
        SHINELOG_INFO("&luckyhour STOP by %s", pk ? pk->GetName().c_str() : "?");
        return true;
    }
    uint32 uiMin     = (uint32)atoi(a[0].c_str());
    int32  iExpX1k   = (a.size() >= 2) ? atoi(a[1].c_str()) : 2000;
    int32  iDropX1k  = (a.size() >= 3) ? atoi(a[2].c_str()) : 2000;
    const uint32 uiDurSec = uiMin * 60;
    if (!WMClient::Get().SendGMEventTrigger(kGMEvent_LuckyHour, uiDurSec, true)) {
        // WM offline -- run the boost locally so the calling zone at
        // least gets a visible event. Cross-zone fanout is forfeit.
        LiveOpsBoosts::Get().StartLuckyHour(uiDurSec, iExpX1k, iDropX1k);
        SHINELOG_WARN("&luckyhour: WM offline, local-only boost on this zone");
    }
    SHINELOG_INFO("&luckyhour START by %s mins=%u exp=%d/1k drop=%d/1k",
                  pk ? pk->GetName().c_str() : "?",
                  uiMin, iExpX1k, iDropX1k);
    return true;
}
const AmpersandCmdEntry kAmpersandCmds[] = {
    { "&hide", 10, &Cmd_default },
    { "&show", 10, &Cmd_default },
    { "&pfind", 10, &Cmd_default },
    { "&mfind", 10, &Cmd_default },
    { "&kickout", 10, &Cmd_default },
    { "&linkto", 10, &Cmd_default },
    { "&psummon", 10, &Cmd_default },
    { "&expbonus", 50, &Cmd_default },
    { "&itembonus", 50, &Cmd_default },
    { "&fieldbonus", 50, &Cmd_default },
    { "&silence", 100, &Cmd_default },
    { "&distribute", 10, &Cmd_default },
    { "&isblock", 10, &Cmd_default },
    { "&mobdist", 10, &Cmd_default },
    { "&chargeditem", 100, &Cmd_default },
    { "&parameterview", 100, &Cmd_default },
    { "&adminlevel", 1, &Cmd_default },
    { "&serverindex", 100, &Cmd_default },
    { "&makeitem", 100, &Cmd_default },
    { "&cleariv", 1, &Cmd_default },
    { "&dimension", 100, &Cmd_default },
    { "&doorbuild", 100, &Cmd_default },
    { "&mobbreed", 50, &Cmd_default },
    { "&mb", 50, &Cmd_default },
    { "&fullpoint", 50, &Cmd_default },
    { "&summonplayer", 100, &Cmd_default },
    { "&performance", 10, &Cmd_default },
    { "&sethp", 100, &Cmd_default },
    { "&setsp", 100, &Cmd_default },
    { "&dist", 10, &Cmd_default },
    { "&coord", 10, &Cmd_default },
    { "&scrint", 10, &Cmd_default },
    { "&timer", 50, &Cmd_default },
    { "&templink", 10, &Cmd_default },
    { "&oxblock", 1, &Cmd_default },
    { "&z", 10, &Cmd_default },
    { "&gmchat", 1, &Cmd_default },
    { "&gldcollectclear", 100, &Cmd_default },
    { "&gldcollectset", 100, &Cmd_default },
    { "&questgiveup", 100, &Cmd_default },
    { "&questrecieve", 100, &Cmd_default },
    { "&questcomplete", 100, &Cmd_default },
    { "&learnskill", 100, &Cmd_default },
    { "&skillcooltime", 100, &Cmd_default },
    { "&setabstate", 100, &Cmd_default },
    { "&resetabstate", 100, &Cmd_default },
    { "&setabstatetarget", 100, &Cmd_default },
    { "&resetabstatetarget", 100, &Cmd_default },
    { "&kqreward", 100, &Cmd_default },
    { "&killallmob", 50, &Cmd_default },
    { "&mobdamage", 100, &Cmd_default },
    { "&suicideallmob", 50, &Cmd_default },
    { "&suicidemob", 100, &Cmd_default },
    { "&kingdomquest", 100, &Cmd_default },
    { "&Reroadkingdomquest", 100, &Cmd_default },
    { "&endofkq", 100, &Cmd_default },
    { "&guildtournament", 100, &Cmd_default },
    { "&zonelink", 100, &Cmd_default },
    { "&block", 100, &Cmd_default },
    { "&emptymap", 100, &Cmd_default },
    { "&serversocketcut", 100, &Cmd_default },
    { "&staticdamage", 50, &Cmd_default },
    { "&deathaura", 100, &Cmd_default },
    { "&immortal", 10, &Cmd_default },
    { "&enterminihouse", 100, &Cmd_default },
    { "&horsespeed", 100, &Cmd_default },
    { "&charinform", 10, &Cmd_default },
    { "&partyinfo", 100, &Cmd_default },
    { "&propose", 100, &Cmd_default },
    { "&proposeres", 100, &Cmd_default },
    { "&weddingreset", 100, &Cmd_default },
    { "&reserve", 100, &Cmd_default },
    { "&enterready", 100, &Cmd_default },
    { "&enter", 100, &Cmd_default },
    { "&learnpassive", 100, &Cmd_default },
    { "&unlearn", 100, &Cmd_default },
    { "&unlearnpassive", 100, &Cmd_default },
    { "&wedding", 100, &Cmd_default },
    { "&showid", 100, &Cmd_default },
    { "&mobhatchery", 100, &Cmd_default },
    { "&allcritical", 100, &Cmd_default },
    { "&erasechargebuf", 100, &Cmd_default },
    { "&startGT", 100, &Cmd_default },
    { "&flagcapture", 100, &Cmd_default },
    { "&useguildskill", 100, &Cmd_default },
    { "&observerlinkto", 100, &Cmd_default },
    { "&observerout", 100, &Cmd_default },
    { "&HP", 100, &Cmd_default },
    { "&SP", 100, &Cmd_default },
    { "&speed", 50, &Cmd_default },
    { "&linktosavedata", 100, &Cmd_default },
    { "&reloaddata", 100, &Cmd_default },
    { "&captivate", 100, &Cmd_default },
    { "&ignoreskillcooltime", 100, &Cmd_default },
    { "&invenview", 100, &Cmd_default },
    { "&runto", 100, &Cmd_default },
    { "&ghostremove", 100, &Cmd_default },
    { "&curtime", 100, &Cmd_default },
    { "&amibusy", 100, &Cmd_default },
    { "&knockback", 100, &Cmd_default },
    { "&partblock", 100, &Cmd_default },
    { "&debuglogging", 100, &Cmd_default },
    { "&&&", 100, &Cmd_default },
    { "&npcrevive", 100, &Cmd_default },
    { "&ts_DiceFix", 100, &Cmd_default },
    { "&ts_DiceProbability", 100, &Cmd_default },
    { "&GB_DiceFix", 100, &Cmd_default },
    { "&GB_DiceProbability", 100, &Cmd_default },
    { "&makeraid", 100, &Cmd_default },
    { "&outraid", 100, &Cmd_default },
    { "&inraid", 100, &Cmd_default },
    { "&PlayIns", 100, &Cmd_default },
    { "&bigbang", 100, &Cmd_default },
    { "&alllearnskill", 100, &Cmd_default },
    { "&levelup", 100, &Cmd_default },
    { "&showallchar", 100, &Cmd_default },
    { "&testlink", 50, &Cmd_default },
    { "&gtieffect", 50, &Cmd_default },
    { "&iareset", 50, &Cmd_default },
    { "&queststart", 50, &Cmd_default },
    { "&questread", 50, &Cmd_default },
    { "&disposition", 50, &Cmd_default },
    { "&idtimechange", 50, &Cmd_default },
    { "&releasechatban", 50, &Cmd_default },
    { "&slotmachine", 1, &Cmd_default },
    { "&godofslotmachine", 100, &Cmd_default },
    { "&viewaggrolist", 50, &Cmd_default },
    { "&ebcu", 50, &Cmd_default },
    { "&luaobserver", 100, &Cmd_default },
    { "&luafuncexec", 100, &Cmd_default },
    { "&mobitemdrop", 100, &Cmd_default },
    { "&getmoney", 100, &Cmd_default },
    { "&fnpc", 100, &Cmd_default },
    { "&RegenNPC", 100, &Cmd_default },
    { "&ReleaseNPC", 100, &Cmd_default },
    { "&gmtestmode", 100, &Cmd_default },
    { "&arrangeiv", 100, &Cmd_default },
    { "&cmt", 100, &Cmd_default },
    { "&reloadid", 100, &Cmd_default },
    { "&vie", 100, &Cmd_default },
    { "&viue", 100, &Cmd_default },
    { "&viuea", 100, &Cmd_default },
    { "&setlp", 100, &Cmd_default },
    { "&movelayer", 100, &Cmd_default },
    { "&moveotherlayer", 100, &Cmd_default },
    { "&godmode", 100, &Cmd_default },
    { "&fieldmobcount", 100, &Cmd_default },
    { "&reloadpetlua", 100, &Cmd_default },
    { "&getpetinfo", 100, &Cmd_default },
    { "&setpetinfo", 100, &Cmd_default },
    { "&vanishdropitem", 100, &Cmd_default },
    { "&walkto", 100, &Cmd_default },
    { "&castteleport", 100, &Cmd_default },
    { "&setobjectdirect", 100, &Cmd_default },
    { "&luckyhour", 100, &Cmd_luckyhour },
    { 0, 0, 0 }
};
const size_t kAmpersandCmdCount = 155;

static std::vector<std::string> Tokenize(const std::string& s) {
    std::vector<std::string> v; std::string cur;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == ' ' || c == '\t') { if (!cur.empty()) { v.push_back(cur); cur.clear(); } }
        else cur.push_back(c);
    }
    if (!cur.empty()) v.push_back(cur);
    return v;
}

bool DispatchAmpersand(ShinePlayer* pk, const std::string& rLine) {
    if (rLine.empty() || rLine[0] != '&') return false;
    std::vector<std::string> tok = Tokenize(rLine);
    if (tok.empty()) return false;
    const std::string& head = tok[0];
    for (size_t i = 0; i < kAmpersandCmdCount; ++i) {
        if (head == kAmpersandCmds[i].szName) {
            // Admin-level gate would compare against pk->GetAdminLv() in pass 2.
            std::vector<std::string> args(tok.begin()+1, tok.end());
            return kAmpersandCmds[i].pkFn(pk, args);
        }
    }
    SHINELOG_WARN("Unknown ampersand command: %s", head.c_str());
    return false;
}
} // namespace shine