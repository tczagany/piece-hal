#ifndef PIECE_HAL_GPIO_H
#define PIECE_HAL_GPIO_H

namespace piece {
namespace hal {

class GPIO {
public:
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

public:
    static void initiate();
    static int getPinsCount();
    static void setPinMode(int index, mode m);
    static mode getPinMode(int index);
    static direction getPinDirection(int index);
    static void setPinDirection(int index, direction d);
    static float getPinValue(int index);
    static void setPinValue(int index, float value);
    static int getTouchValue(int index, int common);
};

}
}

#endif //PIECE_HAL_GPIO_H