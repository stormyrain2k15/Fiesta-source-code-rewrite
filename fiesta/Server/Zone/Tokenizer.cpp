// Server/Zone/Tokenizer.cpp
// String tokenizer -- splits a delimited string into a vector. Used by
// SHN parsers, PineScript, AmpersandCommands.
#include <string>
#include <vector>

namespace fiesta {

class Tokenizer {
public:
    static void Split(const std::string& rIn, char chDelim, std::vector<std::string>& rOut) {
        rOut.clear();
        size_t i = 0;
        while (i <= rIn.size()) {
            size_t j = rIn.find(chDelim, i);
            if (j == std::string::npos) {
                if (i < rIn.size()) rOut.push_back(rIn.substr(i));
                else                rOut.push_back(std::string());
                break;
            }
            rOut.push_back(rIn.substr(i, j - i));
            i = j + 1;
        }
    }
};

} // namespace fiesta
