#ifndef PIECE_HAL_SYSTEM_H
#define PIECE_HAL_SYSTEM_H

#include "device.h"

namespace piece {
namespace hal {

struct ISystem : public IDevice {
    virtual void exit() {}
    virtual void restart() {}
    virtual uint32_t hardwareId() { return 0; }
    virtual uint32_t totalHeapSize() { return 0; }
    virtual uint32_t freeHeapSize() { return 0; }
    virtual uint32_t uptime_ms() { return 0; }
    virtual void wait_ms(uint32_t time) {}
    virtual uint32_t uptime_us() { return 0; }
    virtual void wait_us(uint32_t time) {}
    virtual uint32_t random(int min, int max) { return 0; }
};

}
}

#endif //PIECE_HAL_SYSTEM_H