#include "gpio.h"
#include "gpioconfig.h"

using namespace piece::hal;

namespace piece {
namespace hal_template {

hal::DeviceInfo GPIO::deviceInfo = {
    hal::DeviceType::gpio,
    hal::DeviceClass::unknown,
    "gpio",
    "undefined",
    "GPIO port template"
};

DeviceListEntry GPIO::deviceEntry = {
    nullptr, nullptr, nullptr
};

int GPIO::initiate(variant_t settings) {
    return 0;
}

const hal::DeviceInfo& GPIO::info() {
    return deviceInfo;
}

void GPIO::destroy() {
}

int GPIO::getPinsCount() {
    return 0;
}

void GPIO::setPinMode(int, mode) {
}

IGPIO::mode GPIO::getPinMode(int) {
    return IGPIO::mode::unknown;
}

IGPIO::direction GPIO::getPinDirection(int) {
    return IGPIO::direction::unknown;
}

void GPIO::setPinDirection(int, direction) {
}

float GPIO::getPinValue(int) {
    return 0;
}

void GPIO::setPinValue(int, float) {
}

int GPIO::getTouchValue(int, int) {
    return 0;
}

}
}
