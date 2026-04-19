#ifndef PIECE_HAL_DISPLAY_H
#define PIECE_HAL_DISPLAY_H

#include "gfx/bitmap.h"

namespace piece {
namespace hal {

struct IDisplay {
public:
    virtual void clear() {}
    virtual Bitmap createBitmap() const { return Bitmap{}; }
    virtual void showBitmap(const Bitmap& content) {}
};

}
}

#endif //PIECE_HAL_DISPLAY_H
