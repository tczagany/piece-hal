#include "display.h"

namespace piece {
namespace hal_template {

hal::DeviceInfo Display::deviceInfo = {
    hal::DeviceType::display_1bit,
    hal::DeviceClass::display,
    "display",
    "undefined",
    "Bitmap display template"
};

DeviceListEntry Display::deviceEntry = {
    nullptr, nullptr, nullptr
};

int Display::initiate(variant_t settings) {
    return 0;
}

const hal::DeviceInfo& Display::info() {
    return deviceInfo;
}

void Display::destroy() {
}

void Display::clear() {
}

Bitmap Display::createBitmap() const {
    return Bitmap{};
}

void Display::showBitmap(const Bitmap&) {
}

}
}
