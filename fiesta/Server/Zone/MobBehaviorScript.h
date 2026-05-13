// Server/Zone/MobBehaviorScript.h
// runtime wrapper around a parsed MobBehaviorDescript/*.ps PsScriptFile.
// The DSL drives mob AI choice (which target to chase, when to chat/break, etc).
// This module loads + parses the file via PsScriptFile, then exposes:
//   - the named blocks (e.g. "main", "Regen1stLayer")
//   - a Step(blockName, ctx) entry point that an AI tick can call.
// The per-statement behaviour is implemented inline (chat, mobregen, call,
// break, if/then/else, infinite). Verbs not yet handled return without effect
// but log a one-shot warning -- this lets data drive the AI without aborting.
#ifndef SHINE_ZONE_MOBBEHAVIORSCRIPT_H
#define SHINE_ZONE_MOBBEHAVIORSCRIPT_H
#include "../Shared/ShineTypes.h"
#include "../DataReader/PsScriptFile.h"
#include <map>
#include <string>

namespace shine {

// Per-tick context the runtime hands to the script. Verbs read/write this.
struct MobBehaviorCtx {
    Handle      hSelf;          // the mob handle running this script
    Handle      hCurrentTarget; // populated by 'whoistarget'
    std::string kCurrentTargetName;
    int         iPermillage;    // populated by 'permillage' (HP per-mille)
    Handle      hKilledMe;      // populated by 'whokillme'
    // Free-form variable bag for the script's local 'var' declarations.
    std::map<std::string, std::string> kVars;
    // Outputs requested by the script this tick.
    std::vector<std::string>            kPendingChats;     // verbatim string broadcasts
};

class MobBehaviorScript {
public:
    bool   Load(const std::string& rPath);
    size_t BlockCount() const { return m_kFile.BlockCount(); }
    const  PsScriptFile& File() const { return m_kFile; }

    // Step a named block once. Returns true if the block ran to completion (or
    // hit a 'break' that targets this block).
    bool   Step(const std::string& rBlockName, MobBehaviorCtx& rCtx) const;

private:
    PsScriptFile m_kFile;

    bool   ExecuteBlock(int iBlockIdx, MobBehaviorCtx& rCtx, std::string& rBreakTarget) const;
    bool   EvaluateCond(const PsExpr& rExpr, const MobBehaviorCtx& rCtx) const;
    std::string ResolveExpr(const PsExpr& rExpr, const MobBehaviorCtx& rCtx) const;
};

class MobBehaviorBox {
public:
    static MobBehaviorBox& Get();
    const MobBehaviorScript* LoadDefault(const std::string& rRoot);
    const MobBehaviorScript* LoadKQ     (const std::string& rRoot, const std::string& rKQ);
    const MobBehaviorScript* Find       (const std::string& rKey)  const;
    void Clear();
private:
    std::map<std::string, MobBehaviorScript*> m_kAll;
};

} // namespace shine
#endif
