#ifndef PIECE_HAL_SSID_ENUM_H
#define PIECE_HAL_SSID_ENUM_H

#include "device.h"

namespace piece {
namespace hal {

struct ISSIDEnumerator : public IDevice {
    using enum_cb = void (*)(const char*, const char*, int, int, void*);

    virtual void startQuery(enum_cb cb, void* param) {}
    virtual void cancelQuery() {}
};

}
}

#endif //PIECE_HAL_SSID_ENUM_H
