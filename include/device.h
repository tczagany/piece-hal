#ifndef PIECE_HAL_DEVICE_H
#define PIECE_HAL_DEVICE_H

#include "types.h"

namespace piece {
namespace hal {

enum class DeviceClass : uint8_t {
    unknown = 0,
    sensor = 1,
    actuator = 2,
    transmission = 3,
    display = 4,
    storage = 5,
    system = 6,
    diagnositics = 7
};

enum class DeviceType : uint8_t {
    unknown = 0,
    serial = 1,
    serial_emulator = 2,
    spi = 3,
    i2c = 4,
    gpio = 5,
    adc = 6,
    dac = 7,
    pwm = 8,
    wifi = 9,
    wifi_emulator = 10,
    nrf24 = 11,
    bluetooth = 12,
    ethernet = 13,
    usb = 14,
    sdcard = 15,
    eeprom = 16,
    cpu = 17,
    memory = 18,
    time = 19,
    random = 20,
    flash = 21,
    log_port = 22,
    debug_port = 23,
    display_1bit = 24,
    display_16bit = 25,
    display_24bit = 26,
};

struct DeviceInfo {
    DeviceType deviceType;
    DeviceClass deviceClass;
    const char* name;
    const char* vendor;
    const char* description;
};

struct IDevice {
    virtual int initiate(variant_t settings) = 0;
    virtual const hal::DeviceInfo& info() = 0;
    virtual void destroy() = 0;
};

}
}

#endif //PIECE_HAL_INTERFACE_H
