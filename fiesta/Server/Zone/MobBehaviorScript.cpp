// Server/Zone/MobBehaviorScript.cpp
#include "MobBehaviorScript.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace shine {

bool MobBehaviorScript::Load(const std::string& rPath) {
    return m_kFile.Load(rPath);
}

// Resolve a single expression (one PsExpr can hold a chain such as
// 'Enemy % EnemyName % "abc"' -- we concatenate token text values).
std::string MobBehaviorScript::ResolveExpr(const PsExpr& rExpr, const MobBehaviorCtx& rCtx) const {
    std::string out;
    for (size_t i = 0; i < rExpr.kTokens.size(); ++i) {
        const PsToken& t = rExpr.kTokens[i];
        if (t.eKind == PSTK_PERCENT) continue;        // skip the '%' connector
        if (t.eKind == PSTK_OP)      continue;        // bare operators in concat
        if (t.eKind == PSTK_STRING)  { out += t.kText; continue; }
        if (t.eKind == PSTK_NUMBER)  { out += t.kText; continue; }
        if (t.eKind == PSTK_IDENT) {
            // Variable substitution from the ctx bag.
            std::map<std::string, std::string>::const_iterator it = rCtx.kVars.find(t.kText);
            if (it != rCtx.kVars.end()) out += it->second;
            else                        out += t.kText;
            continue;
        }
        // Other tokens (parens, commas, @-funcs) are passed through verbatim
        // so callers can still inspect them if needed.
        out += t.kText;
    }
    return out;
}

// Evaluate a condition. Supports a small subset:
//   Ident == "string", Ident =!= "string", Ident < N, Ident > N, Ident == 0.
bool MobBehaviorScript::EvaluateCond(const PsExpr& rExpr, const MobBehaviorCtx& rCtx) const {
    // Find the first operator token.
    size_t opIdx = (size_t)-1;
    for (size_t i = 0; i < rExpr.kTokens.size(); ++i)
        if (rExpr.kTokens[i].eKind == PSTK_OP) { opIdx = i; break; }
    if (opIdx == (size_t)-1) {
        // Truthiness of bare identifier: nonempty / non-zero.
        std::string v = ResolveExpr(rExpr, rCtx);
        if (v.empty() || v == "0") return false;
        return true;
    }
    PsExpr lhs; lhs.kTokens.assign(rExpr.kTokens.begin(), rExpr.kTokens.begin() + opIdx);
    PsExpr rhs; rhs.kTokens.assign(rExpr.kTokens.begin() + opIdx + 1, rExpr.kTokens.end());
    std::string op = rExpr.kTokens[opIdx].kText;
    std::string l  = ResolveExpr(lhs, rCtx);
    std::string r  = ResolveExpr(rhs, rCtx);
    if (op == "==" || op == "===") return l == r;
    if (op == "!=" || op == "=!=") return l != r;
    long la = atol(l.c_str()), ra = atol(r.c_str());
    if (op == "<")  return la <  ra;
    if (op == "<=") return la <= ra;
    if (op == ">")  return la >  ra;
    if (op == ">=") return la >= ra;
    return false;
}

bool MobBehaviorScript::ExecuteBlock(int iBlockIdx, MobBehaviorCtx& rCtx, std::string& rBreakTarget) const {
    if (iBlockIdx < 0 || (size_t)iBlockIdx >= m_kFile.BlockCount()) return false;
    const PsBlock& b = m_kFile.Block((size_t)iBlockIdx);

    for (size_t i = 0; i < b.kStmts.size(); ++i) {
        const PsStmt& s = b.kStmts[i];
        if (!rBreakTarget.empty()) {
            if (rBreakTarget == b.kName) { rBreakTarget.clear(); return true; }
            return true;        // bubble up
        }
        switch (s.eKind) {
            case PsStmt::K_VAR_DECL: {
                for (size_t a = 0; a < s.kArgs.size(); ++a) {
                    if (s.kArgs[a].kTokens.empty()) continue;
                    const PsToken& nm = s.kArgs[a].kTokens[0];
                    std::string init;
                    if (s.kArgs[a].kTokens.size() >= 2) init = s.kArgs[a].kTokens[1].kText;
                    rCtx.kVars[nm.kText] = init;
                }
            } break;
            case PsStmt::K_ASSIGN: {
                rCtx.kVars[s.kVerb] = ResolveExpr(s.kCond, rCtx);
            } break;
            case PsStmt::K_IF: {
                bool cond = EvaluateCond(s.kCond, rCtx);
                int target = cond ? s.iThenBlock : s.iElseBlock;
                if (target >= 0) {
                    if (!ExecuteBlock(target, rCtx, rBreakTarget)) return false;
                }
            } break;
            case PsStmt::K_INFINITE: {
                // Cap to a single iteration per Step() invocation. Real engines
                // re-enter via the per-tick caller; nesting deeper would block.
                if (s.iBodyBlock >= 0) {
                    if (!ExecuteBlock(s.iBodyBlock, rCtx, rBreakTarget)) return false;
                }
            } break;
            case PsStmt::K_BREAK: {
                rBreakTarget = s.kVerb; // string from the .ps token
                return true;
            }
            case PsStmt::K_CALL: {
                if (const PsBlock* pk = m_kFile.FindBlock(s.kVerb)) {
                    int idx = (int)(pk - &m_kFile.Block(0));
                    if (!ExecuteBlock(idx, rCtx, rBreakTarget)) return false;
                }
            } break;
            case PsStmt::K_BUILTIN: {
                const std::string& v = s.kVerb;
                // Translate the most common verbs into ctx mutations.
                if (_stricmp(v.c_str(), "whoistarget") == 0 && !s.kArgs.empty()) {
                    const PsToken& nm = s.kArgs[0].kTokens.empty() ? PsToken() : s.kArgs[0].kTokens[0];
                    char buf[16]; sprintf_s(buf, sizeof(buf), "%u", (uint32)rCtx.hCurrentTarget);
                    rCtx.kVars[nm.kText] = (rCtx.hCurrentTarget == INVALID_HANDLE) ? "" : buf;
                } else if (_stricmp(v.c_str(), "whokillme") == 0 && !s.kArgs.empty()) {
                    const PsToken& nm = s.kArgs[0].kTokens.empty() ? PsToken() : s.kArgs[0].kTokens[0];
                    char buf[16]; sprintf_s(buf, sizeof(buf), "%u", (uint32)rCtx.hKilledMe);
                    rCtx.kVars[nm.kText] = (rCtx.hKilledMe == INVALID_HANDLE) ? "" : buf;
                } else if (_stricmp(v.c_str(), "permillage") == 0 && !s.kArgs.empty()) {
                    const PsToken& nm = s.kArgs[0].kTokens.empty() ? PsToken() : s.kArgs[0].kTokens[0];
                    char buf[16]; sprintf_s(buf, sizeof(buf), "%d", rCtx.iPermillage);
                    rCtx.kVars[nm.kText] = buf;
                } else if (_stricmp(v.c_str(), "getname") == 0 && s.kArgs.size() >= 2) {
                    // getname OutVar HandleVar.
                    const PsToken& nm = s.kArgs[0].kTokens.empty() ? PsToken() : s.kArgs[0].kTokens[0];
                    rCtx.kVars[nm.kText] = rCtx.kCurrentTargetName;
                } else if (_stricmp(v.c_str(), "chat") == 0) {
                    // chat <handle> <text>.  Concatenate all arg expressions.
                    std::string line;
                    for (size_t a = 1; a < s.kArgs.size(); ++a) {
                        if (a > 1) line.push_back(' ');
                        line += ResolveExpr(s.kArgs[a], rCtx);
                    }
                    rCtx.kPendingChats.push_back(line);
                }
                // Other verbs (mobregen, doorbuild, broadcast, linkto, etc.) are
                // pass-through for now -- the AI / scenario host wires them.
            } break;
        }
    }
    return true;
}

bool MobBehaviorScript::Step(const std::string& rBlockName, MobBehaviorCtx& rCtx) const {
    const PsBlock* pk = m_kFile.FindBlock(rBlockName);
    if (!pk) return false;
    int idx = (int)(pk - &m_kFile.Block(0));
    std::string brk;
    return ExecuteBlock(idx, rCtx, brk);
}

MobBehaviorBox& MobBehaviorBox::Get() { static MobBehaviorBox s; return s; }

const MobBehaviorScript* MobBehaviorBox::LoadDefault(const std::string& rRoot) {
    std::string path = rRoot + "\\MobBehaviorDescript\\DefaultBehavior.ps";
    MobBehaviorScript* pk = new MobBehaviorScript();
    if (!pk->Load(path)) { delete pk; return NULL; }
    m_kAll["@default"] = pk; return pk;
}
const MobBehaviorScript* MobBehaviorBox::LoadKQ(const std::string& rRoot, const std::string& rKQ) {
    std::string path = rRoot + "\\MobBehaviorDescript\\KQ\\" + rKQ + ".ps";
    MobBehaviorScript* pk = new MobBehaviorScript();
    if (!pk->Load(path)) { delete pk; return NULL; }
    m_kAll[rKQ] = pk; return pk;
}
const MobBehaviorScript* MobBehaviorBox::Find(const std::string& rKey) const {
    std::map<std::string, MobBehaviorScript*>::const_iterator it = m_kAll.find(rKey);
    return (it == m_kAll.end()) ? NULL : it->second;
}
void MobBehaviorBox::Clear() {
    for (std::map<std::string, MobBehaviorScript*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace shine
