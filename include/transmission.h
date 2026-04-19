#ifndef PIECE_HAL_TRANSMISSION_H
#define PIECE_HAL_TRANSMISSION_H

#include "device.h"

namespace piece {
namespace hal {

struct ITransmission : public IDevice {
    using recv_cb = bool (*)(const uint8_t* data, int size, void* param);

    struct statistics {
        int rxMsgCount;
        int rxBytesCount;
        int txMsgCount;
        int txBytesCount;
    };

    virtual int connect(recv_cb callback, void* param) { return 0; }
    virtual int disconnect() { return 0; }
    virtual bool isConnected() { return false; }
    virtual int send(const uint8_t* data, int size) { return 0; }
    virtual void setOption(uint8_t opt) {}
    virtual const statistics* stats() { return nullptr; }
};

}
}

#endif //PIECE_HAL_TRANSMISSION_H
