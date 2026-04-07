#ifndef PIECE_HAL_INTERFACE_H
#define PIECE_HAL_INTERFACE_H

#include "../framework/haldevices.h"

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
    debug_port = 23
};

struct DeviceInfo : DeviceListEntry {
    DeviceType deviceType;
    DeviceClass deviceClass;
    const char* name;
    const char* vendor;
    const char* description;
};

struct IStorage : public IDevice {
    virtual bool mount() = 0;
    virtual void unmount() = 0;
    virtual int free() = 0;
    virtual int size() = 0;
    virtual bool test(const char* path) = 0;
    virtual bool remove(const char* path) = 0;
    virtual int fopen(const char* path) = 0;
    virtual int fcreate(const char* path) = 0;
    virtual void fclose(int file) = 0;
    virtual void fsize(int file) = 0;
    virtual int fread(int file, char* buffer, int size, int offset = 0) = 0;
    virtual int fwrite(int file, char* buffer, int size, int offset = 0) = 0;
    virtual void fflush(int file) = 0;
};

struct ITransmission : public IDevice {
    struct statistics {
        int rxMsgCount;
        int rxBytesCount;
        int txMsgCount;
        int txBytesCount;
    };

    using recv = bool (*)(const uint8_t* data, int size, void* param);

    virtual int connect(recv callback, void* param) = 0;
    virtual int disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual int send(const uint8_t* data, int size) = 0;
    virtual const statistics& stats() = 0;
    virtual void setOption(uint8_t opt) {};
    virtual ~ITransmission() {}
};

}
}

#endif //PIECE_HAL_INTERFACE_H
