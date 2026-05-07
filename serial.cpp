#include "serial.h"

namespace piece {
namespace hal_template {

void Serial::writeTxt(bool) {
}

void Serial::writeTxt(int) {
}

void Serial::writeTxt(float, int) {
}

void Serial::writeTxt(const char*) {
}

void Serial::writeBin(int) {
}

void Serial::writeBin(uint32_t) {
}

void Serial::writeBin(const char*) {
}

void Serial::writeBin(const uint8_t*, int) {
}

void SerialEnumerator::startQuery(enum_cb, void*) {
}

void SerialEnumerator::cancelQuery() {
}

}
}