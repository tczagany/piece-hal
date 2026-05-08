#ifndef PIECE_HAL_SYSTEM_IMPLEMENTATION_H
#define PIECE_HAL_SYSTEM_IMPLEMENTATION_H

#include "piece-fw.h"
#include "hal/system.h"

namespace piece {
namespace hal_template {

struct System : public hal::ISystem {
    int initiate(variant_t settings) override;
    const hal::DeviceInfo& info() override;
    void destroy() override;
    void exit() override;
    void restart() override;
    uint32_t hardwareId() override;
    uint32_t totalHeapSize() override;
    uint32_t freeHeapSize() override;
    uint32_t uptime_ms() override;
    void wait_ms(uint32_t time) override;
    uint32_t uptime_us() override;
    void wait_us(uint32_t time) override;
    uint32_t random(int min, int max) override;

    static hal::DeviceInfo deviceInfo;
    static DeviceListEntry deviceEntry;
};

}
}

#endif //PIECE_HAL_SYSTEM_IMPLEMENTATION_H