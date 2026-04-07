#ifndef PIECE_HAL_DIAGNOSTICS_H
#define PIECE_HAL_DIAGNOSTICS_H

#include "core/strstream.h"

namespace piece {
namespace hal {

class Diag {
public:
    using lineIOCallback = void(*)(char);

    enum class Output {
        null,
        terminal,
        syslog,
        network,
        uart0,
        uart1,
        uart2,
        uart3
    };

    static const uint8_t cnt_time = 1;
    static const uint8_t cnt_nclass = 2;
    static const uint8_t cnt_ninst = 4;
    static const uint8_t cnt_message = 8;

public:
    static void initiate();
    static void set(Output output);
    static void set(uint8_t contentFlags);
    static void set(const char* className, const char* instName);
    static void set(lineIOCallback ioCallback);
    static void write(const char* msg);
    static void write(const uint8_t* buffer, int size);
    static void blink(int count, int delay = 50);

    template<typename... T>
    static void log(const T& ...args);

protected:
    static char frwClassName[32];
    static char frwInstanceName[32];
    static lineIOCallback ioCallback;
    static uint8_t contentFlags;

    class IOStream : public OutputIO {
    public:
        void write(char c) override;
        void flush();
        bool canWrite() override;
        int tell() override { return 0; }
    };

    static void logInternal(str_ostream& oss);
    template<typename A, typename... T>
    static void logInternal(str_ostream& oss, const A& arg, const T& ...args);
};

}
}

#include "diag.hpp"

#endif //PIECE_HAL_DIAGNOSTICS_H
