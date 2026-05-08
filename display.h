#ifndef PIECE_DISPLAY_IMPLEMENTATION_H
#define PIECE_DISPLAY_IMPLEMENTATION_H

#include "piece-fw.h"
#include "hal/display.h"

namespace piece {
namespace hal_template {

struct Display : public hal::IDisplay {
    int initiate(variant_t settings) override;
    const hal::DeviceInfo& info() override;
    void destroy() override;
    void clear() override;
    Bitmap createBitmap() const override;
    void showBitmap(const Bitmap& content) override;

    static hal::DeviceInfo deviceInfo;
    static DeviceListEntry deviceEntry;
};

}
}

#endif //PIECE_DISPLAY_IMPLEMENTATION_H
