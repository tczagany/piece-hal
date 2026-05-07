#ifndef PIECE_HAL_SERIAL_IMPLEMENTATION_H
#define PIECE_HAL_SERIAL_IMPLEMENTATION_H

#include "hal/serial.h"

namespace piece {
namespace hal_template {

class Serial : public hal::ISerial {
public:
    void writeTxt(bool val) override;
    void writeTxt(int num) override;
    void writeTxt(float num, int digits = 2) override;
    void writeTxt(const char* str) override;
    void writeBin(int num) override;
    void writeBin(uint32_t num) override;
    void writeBin(const char* str) override;
    void writeBin(const uint8_t* buffer, int size) override;
};

struct SerialEnumerator : public hal::ISerialEnumerator {
    using enum_cb = void (*)(const char*, void*);

    void startQuery(enum_cb cb, void* param) override;
    void cancelQuery() override;
};

}
}

#endif //PIECE_HAL_SERIAL_H