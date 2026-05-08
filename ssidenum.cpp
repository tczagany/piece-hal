#include "ssidenum.h"

namespace piece {
namespace hal_template {

hal::DeviceInfo SSIDEnumerator::deviceInfo = {
    hal::DeviceType::wifi_emulator,
    hal::DeviceClass::unknown,
    "ssid-enum",
    "unknown",
    "WiFi SSID enumerator template"
};

DeviceListEntry SSIDEnumerator::deviceEntry = {
    nullptr, nullptr, nullptr
};

int SSIDEnumerator::initiate(variant_t settings) {
    return 0;
}

const hal::DeviceInfo& SSIDEnumerator::info() {
    return deviceInfo;
}

void SSIDEnumerator::destroy() {
}

void SSIDEnumerator::startQuery(enum_cb cb, void* param) {
}

void SSIDEnumerator::cancelQuery() {
}

}
}