#ifndef PIECE_HAL_SERIAL_H
#define PIECE_HAL_SERIAL_H

#include "stdint.h"
#include "interface.h"

namespace piece {
namespace hal {

enum class FlowControl {
    none,
    windowing,
    xon_xoff,
    hardware
};

class Serial {
public:
    static void initiate();
    static void writeTxt(int num);
    static void writeTxt(float num, int digits = 2);
    static void writeTxt(const char* str);
    static void writeBin(int num);
    static void writeBin(uint32_t num);
    static void writeBin(const char* str);
    static void writeBin(const uint8_t* buffer, int size);
    static int readBytes(uint8_t* buffer, int size);
};

class ISerialDevice : public ITransmission {
public:
    using portEnumCallback = void (*)(const string&, void*);

public:
    virtual void writeTxt(bool val) = 0;
    virtual void writeTxt(int num) = 0;
    virtual void writeTxt(float num, int digits = 2) = 0;
    virtual void writeTxt(const char* str) = 0;
    virtual void writeBin(int num) = 0;
    virtual void writeBin(uint32_t num) = 0;
    virtual void writeBin(const char* str) = 0;
    virtual void writeBin(const uint8_t* buffer, int size) = 0;
    virtual void enumeratePorts(void* param, portEnumCallback cb) = 0;
};

}
}

#endif //PIECE_HAL_SERIAL_H