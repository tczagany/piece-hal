#ifndef PIECE_HAL_DIAGSTREAM_IMPLEMENTATION_H
#define PIECE_HAL_DIAGSTREAM_IMPLEMENTATION_H

#include "hal/diagstream.h"

namespace piece {
namespace hal_template {

struct DiagStream : public hal::IDiagStream {
    bool canWrite() override;
    void flush() override;
    void write(char chr) override;
    void write(const char* msg) override;
    void write(const uint8_t* buffer, int size) override;
    void hwIndicator(int count, int delay) override;
};

}
}

#endif //PIECE_HAL_DIAGSTREAM_IMPLEMENTATION_H