#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <thread>

#include "piece.h"
#include "hal/system.h"

namespace piece {
namespace hal {

void registerWiFiDevice();
void registerWiFiEmulatorDevice();
void registerSerialDevice();
void registerSerialEmulatorDevice();
void registerSSIDRetrieverDevice();

static bool randInit = true;
static std::chrono::steady_clock::time_point start = std::chrono::steady_clock::time_point{};

void System::initiate() {
    registerWiFiDevice();
    registerWiFiEmulatorDevice();
    registerSerialDevice();
    registerSerialEmulatorDevice();
    registerSSIDRetrieverDevice();
}

void System::quit() {
    exit(0);
}

void System::reset() {
}

uint32_t System::hardwareId() {
    return hash::calculate(getpid());
}

uint32_t System::heapSize() {
    return 1;
}

uint32_t System::freeHeapSize() {
    return 0;
}

uint32_t System::uptime_ms() {
    auto now = std::chrono::steady_clock::now();
    if (start.time_since_epoch().count() == 0)
        start = now;
    auto dur = now - start;
    return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}

uint32_t System::uptime_us() {
    auto now = std::chrono::steady_clock::now();
    if (start.time_since_epoch().count() == 0)
        start = now;
    auto dur = now - start;
    return (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
}

void System::wait_ms(uint32_t time) {
    std::this_thread::sleep_for(std::chrono::milliseconds{time});
}

uint32_t System::random(int min, int max) {
    if (randInit) {
        randInit = false;
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        srand(now);
    }
    return min == max ? min : min + (rand() % (max - min));
}

}
}
