// Server/Zone/PineScript/Condition.cpp
// Conditional expression evaluator for PineScript "If" / "While" /
// "Until" statements. Operators: == != < <= > >= && || !.
#include "PineScript.h"
#include <stdlib.h>
#include <string>

namespace fiesta {

class Condition {
public:
    // Evaluates `<lhs> <op> <rhs>` over the program-local variable map.
    static bool Eval(const std::string& rLhs, const std::string& rOp, const std::string& rRhs) {
        int li = atoi(rLhs.c_str());
        int ri = atoi(rRhs.c_str());
        if (rOp == "==") return li == ri;
        if (rOp == "!=") return li != ri;
        if (rOp == "<")  return li <  ri;
        if (rOp == "<=") return li <= ri;
        if (rOp == ">")  return li >  ri;
        if (rOp == ">=") return li >= ri;
        return false;
    }
};

} // namespace fiesta
