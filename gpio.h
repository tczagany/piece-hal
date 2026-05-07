#ifndef PIECE_GPIO_IMPLEMENTATION_H
#define PIECE_GPIO_IMPLEMENTATION_H

#include "hal/gpio.h"

namespace piece {
namespace hal_template {

struct GPIO : public hal::IGPIO {
    int getPinsCount() override;
    void setPinMode(int index, mode m) override;
    mode getPinMode(int index) override;
    direction getPinDirection(int index) override;
    void setPinDirection(int index, direction d) override;
    float getPinValue(int index) override;
    void setPinValue(int index, float value) override;
    int getTouchValue(int index, int common) override;
};

}
}

#endif //PIECE_GPIO_IMPLEMENTATION_H