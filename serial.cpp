#include "serial.h"

namespace piece {
namespace hal_template {

hal::DeviceInfo Serial::deviceInfo = {
    hal::DeviceType::serial,
    hal::DeviceClass::unknown,
    "serialport",
    "unknown",
    "Serial port template"
};

DeviceListEntry Serial::deviceEntry = {
    nullptr, nullptr, nullptr
};

hal::DeviceInfo SerialEnumerator::deviceInfo = {
    hal::DeviceType::serial,
    hal::DeviceClass::unknown,
    "serial-enum",
    "unknown",
    "Serial port enumerator template"
};

DeviceListEntry SerialEnumerator::deviceEntry = {
    nullptr, nullptr, nullptr
};

int Serial::initiate(variant_t settings) {
    return 0;
}

const hal::DeviceInfo& Serial::info() {
    return deviceInfo;
}

void Serial::destroy() {
}

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

int SerialEnumerator::initiate(variant_t settings) {
    return 0;
}

const hal::DeviceInfo& SerialEnumerator::info() {
    return deviceInfo;
}

void SerialEnumerator::destroy() {
}

void SerialEnumerator::startQuery(enum_cb, void*) {
}

void SerialEnumerator::cancelQuery() {
}

}
}