#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <syslog.h>

#include "hal/diag.h"

namespace piece {
namespace hal {

static int logBufferPos = 0;
static char logBuffer[4096];
static Diag::Output diagPort = Diag::Output::terminal;

void Diag::IOStream::write(char c) {
    if (diagPort == Diag::Output::terminal)
        std::cout << c;
    if (diagPort == Diag::Output::syslog) {
        logBuffer[logBufferPos++] = c;
        logBuffer[logBufferPos] = 0;
    }
    if (diagPort == Diag::Output::network) {
        if (Diag::ioCallback != nullptr)
            ioCallback(c);
    }
}

void Diag::IOStream::flush() {
    if (diagPort == Diag::Output::terminal)
        std::cout.flush();
    if (diagPort == Diag::Output::syslog) {
        syslog(LOG_INFO, "%s", logBuffer);
        logBufferPos = 0;
        logBuffer[0] = 0;
    }
    if (diagPort == Diag::Output::network) {
        if (Diag::ioCallback != nullptr)
            ioCallback(0);
    }
}

bool Diag::IOStream::canWrite() {
    return logBufferPos < sizeof(logBuffer) - 2;
}

void Diag::initiate() {
    set(hal::Diag::cnt_time | hal::Diag::cnt_nclass |
        hal::Diag::cnt_ninst | hal::Diag::cnt_message);
}

void Diag::set(Output dport) {
    diagPort = dport;
}

void Diag::blink(int count, int delay) {
}

void Diag::write(const char* content) {
    if (diagPort == Diag::Output::terminal) {
        std::cout << content;
        std::cout.flush();
    }
    if (diagPort == Diag::Output::syslog)
        syslog(LOG_INFO, "%s", content);
}

}
}
