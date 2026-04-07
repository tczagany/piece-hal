#ifndef PIECE_HAL_SYSTEM_H
#define PIECE_HAL_SYSTEM_H

#include <cstdint>

namespace piece {
namespace hal {

class System {
public:
    static void initiate();
    static void quit();
    static void reset();
    static uint32_t hardwareId();
    static uint32_t uptime_ms();
    static uint32_t uptime_us();
    static uint32_t heapSize();
    static uint32_t freeHeapSize();
    static void wait_ms(uint32_t time);
    static uint32_t random(int min, int max);
};

}
}

#endif //PIECE_HAL_SYSTEM_H