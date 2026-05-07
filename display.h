#ifndef PIECE_DISPLAY_IMPLEMENTATION_H
#define PIECE_DISPLAY_IMPLEMENTATION_H

#include "hal/display.h"

namespace piece {
namespace hal_template {

struct Display : public hal::IDisplay {
    void clear() override;
    Bitmap createBitmap() const override;
    void showBitmap(const Bitmap& content) override;
};

}
}

#endif //PIECE_DISPLAY_IMPLEMENTATION_H
