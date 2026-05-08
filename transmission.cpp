#include "transmission.h"

namespace piece {
namespace hal_template {

hal::DeviceInfo Transmission::deviceInfo = {
    hal::DeviceType::unknown,
    hal::DeviceClass::transmission,
    "network",
    "unknown",
    "Generic byte oriented transmission template"
};

DeviceListEntry Transmission::deviceEntry = {
    nullptr, nullptr, nullptr
};

int Transmission::initiate(variant_t settings) {
    return 0;
}

const hal::DeviceInfo& Transmission::info() {
    return deviceInfo;
}

void Transmission::destroy() {
}

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