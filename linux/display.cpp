#include "hal/display.h"

namespace piece {
namespace hal {

class DisplayEmulator : public AbstractDisplay {
public:
public:
    DisplayEmulator(const char*, int sx, int sy) {
        width = sx, height = sy;
    }

public:
    void clear() override {}
    void showBitmap(const Bitmap &content) override {}

    Bitmap createCompatibleBitmap() const override {
        return Bitmap{ width, height, BitmapMode::mode_1bit};
    }

protected:
    int width;
    int height;
};

AbstractDisplay* DisplayFactory::create(const char* driver, int w, int h) {
    return new DisplayEmulator(driver, w, h);
}

}
}