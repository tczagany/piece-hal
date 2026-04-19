#ifndef PIECE_HAL_DIAGSTREAM_H
#define PIECE_HAL_DIAGSTREAM_H

#include "core/abstractio.h"
#include "device.h"

namespace piece {
namespace hal {

struct IDiagStream : public IDevice, public OutputIO {
    virtual bool canWrite() { return false; }
    virtual void flush() {}
    virtual void write(char chr) {}
    virtual void write(const char* msg) {}
    virtual void write(const uint8_t* buffer, int size) {}
    virtual void indicator(int count, int delay) {}
};

}
}

#endif //PIECE_HAL_DIAGSTREAM_H
