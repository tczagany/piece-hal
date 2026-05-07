#include "transmission.h"

namespace piece {
namespace hal_template {

int Transmission::connect(recv_cb, void*) {
    return 0;
}

int Transmission::disconnect() {
    return 0;
}

bool Transmission::isConnected() {
    return false;
}

int Transmission::send(const uint8_t*, int) {
    return 0;
}

void Transmission::setOption(uint8_t) {
}

const Transmission::statistics* Transmission::stats() {
    static statistics dummy;
    return &dummy;
}

}
}