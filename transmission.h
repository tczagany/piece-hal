#ifndef PIECE_HAL_TRANSMISSION_IMPLEMENTATION_H
#define PIECE_HAL_TRANSMISSION_IMPLEMENTATION_H

#include "piece-fw.h"
#include "hal/transmission.h"

namespace piece {
namespace hal_template {

struct Transmission : public hal::ITransmission {
    int initiate(variant_t settings) override;
    const hal::DeviceInfo& info() override;
    void destroy() override;
    int connect(recv_cb callback, void* param) override;
    int disconnect() override;
    bool isConnected() override;
    int send(const uint8_t* data, int size) override;
    void setOption(uint8_t opt) override;
    const statistics* stats() override;

    static hal::DeviceInfo deviceInfo;
    static DeviceListEntry deviceEntry;
};

}
}

#endif //PIECE_HAL_TRANSMISSION_IMPLEMENTATION_H