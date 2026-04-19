#ifndef PIECE_HAL_GPIO_H
#define PIECE_HAL_GPIO_H

#include "device.h"

namespace piece {
namespace hal {

struct IGPIO : public IDevice{
    enum class direction {
        unknown,
        input,
        output
    };

    enum class mode {
        unknown,
        digital,
        analog,
        touch
    };

    virtual int getPinsCount() { return 0; }
    virtual void setPinMode(int index, mode m) {}
    virtual mode getPinMode(int index) { return mode::unknown; }
    virtual direction getPinDirection(int index) { return direction::unknown; }
    virtual void setPinDirection(int index, direction d) {}
    virtual float getPinValue(int index) { return 0; }
    virtual void setPinValue(int index, float value) {}
    virtual int getTouchValue(int index, int common) { return 0; }
};

}
}

#endif //PIECE_HAL_GPIO_H