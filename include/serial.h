#ifndef PIECE_HAL_SERIAL_H
#define PIECE_HAL_SERIAL_H

#include "transmission.h"

namespace piece {
namespace hal {

enum class FlowControl {
    none,
    windowing,
    xon_xoff,
    hardware
};

class ISerial : public ITransmission {
public:
    virtual void writeTxt(bool val) {}
    virtual void writeTxt(int num) {}
    virtual void writeTxt(float num, int digits = 2) {}
    virtual void writeTxt(const char* str) {}
    virtual void writeBin(int num) {}
    virtual void writeBin(uint32_t num) {}
    virtual void writeBin(const char* str) {}
    virtual void writeBin(const uint8_t* buffer, int size) {}
};

struct ISerialEnumerator : public IDevice {
    using enum_cb = void (*)(const char*, void*);

    virtual void startQuery(enum_cb cb, void* param) {}
    virtual void cancelQuery() {}
};

}
}

#endif //PIECE_HAL_SERIAL_H