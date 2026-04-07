#include "hal/gpio.h"
#include "config.h"
#include "types.h"

namespace piece {
namespace hal {

void GPIO::initiate() {
}

int GPIO::getPinsCount() {
    return GPIO_PIN_COUNT;
}

void GPIO::setPinMode(int index, mode m) {
    variant params;
    *params["pin"_cs] = index;
    *params["mode"_cs] = m == mode::digital ? "digital"_cs : "analog"_cs;
}

GPIO::mode GPIO::getPinMode(int index) {
    return mode::unknown;
}

GPIO::direction GPIO::getPinDirection(int index) {
    return direction::unknown;
}

void GPIO::setPinDirection(int index, direction d) {
}

float GPIO::getPinValue(int index) {
    return 0;
}

void GPIO::setPinValue(int index, float value) {
}

int GPIO::getTouchValue(int index, int ctrl) {
    return 0;
}

}
}
