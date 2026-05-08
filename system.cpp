#include "system.h"

namespace piece {
namespace hal_template {

hal::DeviceInfo System::deviceInfo = {
    hal::DeviceType::unknown,
    hal::DeviceClass::unknown,
    "system",
    "unknown",
    "General system service template"
};

DeviceListEntry System::deviceEntry = {
    nullptr, nullptr, nullptr
};

int System::initiate(variant_t settings) {
    return 0;
}

const hal::DeviceInfo& System::info() {
    return deviceInfo;
}

void System::destroy() {
}

void System::exit() {
}

void System::restart() {
}

uint32_t System::hardwareId() {
    return 0;
}

uint32_t System::totalHeapSize() {
    return 0;
}

uint32_t System::freeHeapSize() {
    return 0;
}

uint32_t System::uptime_ms() {
    return 0;
}

void System::wait_ms(uint32_t) {
}

uint32_t System::uptime_us() {
    return 0;
}

void System::wait_us(uint32_t) {
}

uint32_t System::random(int, int) {
    return 0;
}

}
}
