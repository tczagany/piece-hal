#include "display.h"

namespace piece {
namespace hal_template {

void Display::clear() {
}

Bitmap Display::createBitmap() const {
    return Bitmap{};
}

void Display::showBitmap(const Bitmap&) {
}

}
}
