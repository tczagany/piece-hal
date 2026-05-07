#include "gpio.h"
#include "gpioconfig.h"

using namespace piece::hal;

namespace piece {
namespace hal_template {

int getPinsCount() {
    return 0;
}

void setPinMode(int, IGPIO::mode) {
}

IGPIO::mode getPinMode(int) {
    return IGPIO::mode::unknown;
}

IGPIO::direction getPinDirection(int) {
    return IGPIO::direction::unknown;
}

void setPinDirection(int, IGPIO::direction) {
}

float getPinValue(int) {
    return 0;
}

void setPinValue(int, float) {
}

int getTouchValue(int, int) {
    return 0;
}

}
}
