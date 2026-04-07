#ifndef PIECE_HAL_DISPLAY_H
#define PIECE_HAL_DISPLAY_H

#include "gfx/bitmap.h"

namespace piece {
namespace hal {

class AbstractDisplay {
public:
    virtual void clear() = 0;
    virtual Bitmap createCompatibleBitmap() const = 0;
    virtual void showBitmap(const Bitmap& content) = 0;
};

class DisplayFactory {
public:
    static AbstractDisplay* create(const char* driver, int w, int h);
};

}
}

#endif //PIECE_HAL_DISPLAY_H
