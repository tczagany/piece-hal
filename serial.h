#ifndef PIECE_HAL_SERIAL_IMPLEMENTATION_H
#define PIECE_HAL_SERIAL_IMPLEMENTATION_H

#include "piece-fw.h"
#include "hal/serial.h"

namespace piece {
namespace hal_template {

class Serial : public hal::ISerial {
public:
    int initiate(variant_t settings) override;
    const hal::DeviceInfo& info() override;
    void destroy() override;
    void writeTxt(bool val) override;
    void writeTxt(int num) override;
    void writeTxt(float num, int digits = 2) override;
    void writeTxt(const char* str) override;
    void writeBin(int num) override;
    void writeBin(uint32_t num) override;
    void writeBin(const char* str) override;
    void writeBin(const uint8_t* buffer, int size) override;

    static hal::DeviceInfo deviceInfo;
    static DeviceListEntry deviceEntry;
};

struct SerialEnumerator : public hal::ISerialEnumerator {
    int initiate(variant_t settings) override;
    const hal::DeviceInfo& info() override;
    void destroy() override;
    void startQuery(enum_cb cb, void* param) override;
    void cancelQuery() override;

    static hal::DeviceInfo deviceInfo;
    static DeviceListEntry deviceEntry;
};

}
}

#endif //PIECE_HAL_SERIAL_IMPLEMENTATION_H