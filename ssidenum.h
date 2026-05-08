#ifndef PIECE_HAL_SSIDENUM_IMPLEMENTATION_H
#define PIECE_HAL_SSIDENUM_IMPLEMENTATION_H

#include "piece-fw.h"
#include "hal/ssidenum.h"

namespace piece {
namespace hal_template {

struct SSIDEnumerator : public hal::ISSIDEnumerator {
    int initiate(variant_t settings) override;
    const hal::DeviceInfo& info() override;
    void destroy() override;
    void startQuery(enum_cb cb, void* param) override;
    void cancelQuery() override;

    static hal::DeviceInfo deviceInfo;
    static DeviceListEntry deviceEntry;
};

}
}

#endif //PIECE_HAL_SSIDENUM_IMPLEMENTATION_H