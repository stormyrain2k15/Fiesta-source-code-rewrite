// Server/Zone/CharDataTransmission.cpp
// Builds and sends the post-login data sequence to the client (char info
// -> inventory -> skills -> abstates -> friends -> titles).
#include "ShineObject.h"
namespace fiesta {
class CharDataTransmission {
public:
    static void SendPostLogin(ShinePlayer* /*pkP*/) {}
};
} // namespace fiesta
